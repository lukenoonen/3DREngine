#include "CubemapResource.h"
#include "RenderManager.h"
#include "ResourceManager.h"

CCubemapResource::CCubemapResource( SCubemapResourceData &sData, const char *sName ) : BaseClass( sName )
{
	glGenTextures( 1, &m_glID );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_glID );

	for (unsigned char i = 0; i < 6; i++)
	{
		GLint tInternalFormat = GL_RGBA8;
		GLenum tFormat = GL_RGBA;

		switch (sData.ucChannels[i])
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

		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, tInternalFormat, sData.iWidth[i], sData.iHeight[i], 0, tFormat, GL_UNSIGNED_BYTE, sData.pData[i] );
	}
		
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, sData.bFiltered ? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, sData.bFiltered ? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
}

CCubemapResource::~CCubemapResource()
{
	pRenderManager->UnbindTexture( m_glID );
	glDeleteTextures( 1, &m_glID );
}

int CCubemapResource::Bind( void ) const
{
	return pRenderManager->BindTexture( m_glID, GL_TEXTURE_CUBE_MAP );
}

bool UTIL_Write( CFile *pFile, CCubemapResource *&pData )
{
	const char *sName = pData->GetName();
	return pFile->Write( sName );
}

bool UTIL_Read( CFile *pFile, CCubemapResource *&pData )
{
	char *sName;
	if (!pFile->Read( sName ))
		return false;

	pData = pResourceManager->GetCubemapResource( sName );
	delete[] sName;
	return pData != NULL;
}

bool UTIL_GetValue( const CTextItem *pTextItem, CCubemapResource *&pValue )
{
	const char *sName;
	if (!pTextItem->GetValue( sName ))
		return false;

	pValue = pResourceManager->GetCubemapResource( sName );
	return pValue != NULL;
}