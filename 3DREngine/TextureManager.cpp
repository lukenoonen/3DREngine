#include "TextureManager.h"
#include <UTIL.h>
#include "stb_image.h"
#include <glad/glad.h>
#include <iostream>

CTextureManager::CTextureManager()
{
    stbi_set_flip_vertically_on_load( true );
	glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &m_iMaxTextures );
	glActiveTexture( GetGenerateTexture() );
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
		if (UTIL_strcmp( m_pTextures[i]->GetPath(), sPath ) == 0)
			return m_pTextures[i];
	}

	CTexture *pNewTexture = CreateTexture( sPath );
	m_pTextures.push_back( pNewTexture );
	return pNewTexture;
}

CTexture *CTextureManager::GetCubemap( const char *sPaths[] )
{
	for (unsigned int i = 0; i < m_pTextures.size(); i++)
	{
		if (UTIL_strcmp( m_pTextures[i]->GetPath(), sPaths[0] ) == 0)
			return m_pTextures[i];
	}

	CTexture *pNewTexture = CreateCubemap( sPaths );
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

	int iWidth, iHeight, nrComponents;
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

		glBindTexture( GL_TEXTURE_2D, uiTextureID );
		glTexImage2D( GL_TEXTURE_2D, 0, tInternalFormat, iWidth, iHeight, 0, tFormat, GL_UNSIGNED_BYTE, pData );
		glGenerateMipmap( GL_TEXTURE_2D );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

		stbi_image_free( pData );
	}
	else
	{
		std::cout << "Texture failed to load at path: " << sPath << std::endl;
		stbi_image_free( pData );
	}

	return new CTexture( uiTextureID, sPath );
}

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
			stbi_image_free( pData );
		}
		else
		{
			std::cout << "Texture failed to load at path: " << sPaths[i] << std::endl;
			stbi_image_free( pData );
		}
	}

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );

	return new CTexture( uiTextureID, sPaths[0] );
}