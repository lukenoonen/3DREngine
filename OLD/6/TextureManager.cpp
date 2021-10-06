#include "TextureManager.h"
#include <UTIL.h>
#include <glad/glad.h>
#include <iostream>
#include "TextureWrap.h"
#include <fstream>

CTextureManager::CTextureManager()
{
	glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_iMaxTextures );
	glActiveTexture( GetGenerateTexture() );

	m_iTextureIndex = 0;
}

CTextureManager::~CTextureManager()
{
	Reset();
}

void CTextureManager::Reset( void )
{
	m_iTextureIndex = 0;

	for (unsigned int i = 0; i < m_pTextures.size(); i++)
		delete m_pTextures[i];

	m_pTextures.clear();

	m_mapTextureIDToIndex.clear();
	m_mapIndexToTextureID.clear();
}

CTexture *CTextureManager::GetTexture( const char *sPath )
{
	for (unsigned int i = 0; i < m_pTextures.size(); i++)
	{
		CTexture *pTexture = m_pTextures[i];
		if (UTIL_streq( pTexture->GetPath(), sPath ))
			return pTexture;
	}

	CTexture *pNewTexture = CreateTexture( sPath );
	m_pTextures.push_back( pNewTexture );
	return pNewTexture;
}

int CTextureManager::BindTexture( unsigned int uiTextureID, unsigned int uiTextureType )
{
	std::unordered_map<unsigned int, int>::iterator itTextureIDSearch = m_mapTextureIDToIndex.find( uiTextureID );
	if (itTextureIDSearch == m_mapTextureIDToIndex.end())
	{
		int iPrevTextureIndex = m_iTextureIndex;
		m_iTextureIndex = (m_iTextureIndex + 1) % (GetMaxTextures() - 1);

		std::unordered_map<int, unsigned int>::iterator itIndexSearch = m_mapIndexToTextureID.find( iPrevTextureIndex );
		if (itIndexSearch != m_mapIndexToTextureID.end())
		{
			m_mapTextureIDToIndex.erase( itIndexSearch->second );
			m_mapIndexToTextureID.erase( itIndexSearch );
		}

		m_mapTextureIDToIndex.emplace( uiTextureID, iPrevTextureIndex );
		m_mapIndexToTextureID.emplace( iPrevTextureIndex, uiTextureID );

		glActiveTexture( GL_TEXTURE0 + iPrevTextureIndex );
		glBindTexture( uiTextureType, uiTextureID );
		glActiveTexture( GetGenerateTexture() );

		return iPrevTextureIndex;
	}

	return itTextureIDSearch->second;
}

void CTextureManager::UnbindTexture( unsigned int uiTextureID )
{
	std::unordered_map<unsigned int, int>::iterator itTextureIDSearch = m_mapTextureIDToIndex.find( uiTextureID );
	if (itTextureIDSearch != m_mapTextureIDToIndex.end())
	{
		m_mapTextureIDToIndex.erase( itTextureIDSearch );
		m_mapIndexToTextureID.erase( itTextureIDSearch->second );
	}
}

void CTextureManager::UnbindAllTextures( void )
{
	m_mapTextureIDToIndex.clear();
	m_mapIndexToTextureID.clear();
	m_iTextureIndex = 0;
}

int CTextureManager::GetMaxTextures( void ) const
{
	return m_iMaxTextures;
}

int CTextureManager::GetGenerateTexture( void ) const
{
	return GL_TEXTURE0 + m_iMaxTextures - 1;
}

CTexture *CTextureManager::CreateTexture( const char *sPath )
{
	char sFullPath[260];
	sprintf_s( sFullPath, sizeof(sFullPath), "resources/textures/%s", sPath );

	unsigned int uiTextureID;
	glGenTextures( 1, &uiTextureID );

	std::fstream fTexture( sFullPath, std::ios::in | std::ios::binary );
	if (!fTexture.is_open())
		ExitError( "Cannot open texture file." );

	bool bCubemap;
	UTIL_Read( fTexture, &bCubemap, 1, bool );

	bool bFilter;
	UTIL_Read( fTexture, &bFilter, 1, bool );

	GLint tFilterType = bFilter ? GL_LINEAR : GL_NEAREST;

	if (bCubemap)
	{
		for (unsigned int i = 0; i < 6; i++)
		{
			int iWidth;
			int iHeight;
			int iChannels;
			unsigned char *pData;

			char *sImagePath;
			UTIL_ReadString( fTexture, sImagePath );
			pData = ReadImage( sImagePath, iWidth, iHeight, iChannels );
			delete[] sImagePath;

			GLenum tInternalFormat;
			GLenum tFormat;
			switch (iChannels)
			{
			case 1:
				tInternalFormat = GL_R8;
				tFormat = GL_RED;
				break;
			case 2:
				tInternalFormat = GL_RG8;
				tFormat = GL_RG;
				break;
			case 3:
				tInternalFormat = GL_RGB8;
				tFormat = GL_RGB;
				break;
			case 4:
				tInternalFormat = GL_RGBA8;
				tFormat = GL_RGBA;
				break;
			}

			glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, tInternalFormat, iWidth, iHeight, 0, tFormat, GL_UNSIGNED_BYTE, pData );

			delete[] pData;
		}

		fTexture.close();

		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, tFilterType );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, tFilterType );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	}
	else
	{
		TextureWrap_t tTextureWrap;
		glm::vec4 vecBorderColor;
		int iWidth;
		int iHeight;
		int iChannels;
		unsigned char *pData;

		UTIL_Read( fTexture, &tTextureWrap, 1, TextureWrap_t );
		if (tTextureWrap == TEXTUREWRAP_BORDER)
			UTIL_Read( fTexture, &vecBorderColor, 1, glm::vec4 );

		char *sImagePath;
		UTIL_ReadString( fTexture, sImagePath );
		pData = ReadImage( sImagePath, iWidth, iHeight, iChannels );
		delete[] sImagePath;

		fTexture.close();

		GLenum tInternalFormat;
		GLenum tFormat;
		switch (iChannels)
		{
		case 1:
			tInternalFormat = GL_R8;
			tFormat = GL_RED;
			break;
		case 2:
			tInternalFormat = GL_RG8;
			tFormat = GL_RG;
			break;
		case 3:
			tInternalFormat = GL_RGB8;
			tFormat = GL_RGB;
			break;
		case 4:
			tInternalFormat = GL_RGBA8;
			tFormat = GL_RGBA;
			break;
		}

		GLint tTextureWrapType;
		switch (tTextureWrap)
		{
		case TEXTUREWRAP_REPEAT:
			tTextureWrapType = GL_REPEAT;
			break;
		case TEXTUREWRAP_MIRRORED:
			tTextureWrapType = GL_MIRRORED_REPEAT;
			break;
		case TEXTUREWRAP_EDGE:
			tTextureWrapType = GL_CLAMP_TO_EDGE;
			break;
		case TEXTUREWRAP_BORDER:
			tTextureWrapType = GL_CLAMP_TO_BORDER;
			break;
		}

		glBindTexture( GL_TEXTURE_2D, uiTextureID );
		glTexImage2D( GL_TEXTURE_2D, 0, tInternalFormat, iWidth, iHeight, 0, tFormat, GL_UNSIGNED_BYTE, pData );
		glGenerateMipmap( GL_TEXTURE_2D );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tTextureWrapType );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tTextureWrapType );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, tFilterType );
		if (tTextureWrap == TEXTUREWRAP_BORDER)
			glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr( vecBorderColor ) );

		delete[] pData;
	}

	return new CTexture( uiTextureID, sPath );
}
/*
CTexture *CTextureManager::CreateCubemap( const char **sPaths )
{
	unsigned int uiTextureID;
	glGenTextures( 1, &uiTextureID );
	glBindTexture( GL_TEXTURE_CUBE_MAP, uiTextureID );

	int iWidth, iHeight, nrComponents;
	for (unsigned int i = 0; i < 6; i++)
	{
		char sFullPath[260];
		sprintf_s( sFullPath, sizeof(sFullPath), "resources/textures/%s", sPaths[i] );

		unsigned char *pData = stbi_load( sFullPath, &iWidth, &iHeight, &nrComponents, 0 );
		if (pData)
		{
			GLenum tInternalFormat;
			GLenum tFormat;
			switch (nrComponents)
			{
			case 1:
				tInternalFormat = GL_R8;
				tFormat = GL_RED;
				break;
			case 2:
				tInternalFormat = GL_RG8;
				tFormat = GL_RG;
				break;
			case 3:
				tInternalFormat = GL_RGB8;
				tFormat = GL_RGB;
				break;
			case 4:
				tInternalFormat = GL_RGBA8;
				tFormat = GL_RGBA;
				break;
			}

			glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, tInternalFormat, iWidth, iHeight, 0, tFormat, GL_UNSIGNED_BYTE, pData );
		}
		else
		{
			std::cout << "Texture failed to load at path: " << sPaths[i] << std::endl;
		}
	}

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	return new CTexture( uiTextureID, sPaths[0] );
}*/

unsigned char *CTextureManager::ReadImage( const char *sPath, int &iWidth, int &iHeight, int &iChannels )
{
	char sFullPath[260];
	sprintf_s( sFullPath, sizeof( sFullPath ), "resources/images/%s", sPath );

	std::fstream fImage( sFullPath, std::ios::in | std::ios::binary );
	if (!fImage.is_open())
		ExitError( "Cannot open image file." );

	UTIL_Read( fImage, &iWidth, 1, int );
	UTIL_Read( fImage, &iHeight, 1, int );
	UTIL_Read( fImage, &iChannels, 1, int );

	unsigned char *pData = new unsigned char[iWidth * iHeight * iChannels];
	UTIL_Read( fImage, pData, iWidth * iHeight * iChannels, unsigned char );

	fImage.close();

	return pData;
}