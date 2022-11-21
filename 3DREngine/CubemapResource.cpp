#include "CubemapResource.h"
#include "RenderManager.h"
#include "ResourceManager.h"

CCubemapResource::CCubemapResource( unsigned char **pData, GLsizei *glWidth, GLsizei *glHeight, GLenum *tInternalFormat, GLenum *tFormat, bool bFiltered, const char *sName ) : BaseClass( sName )
{
	glGenTextures( 1, &m_glID );
	glBindTexture( GL_TEXTURE_CUBE_MAP, m_glID );
	for (unsigned char i = 0; i < 6; i++)
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, tInternalFormat[i], glWidth[i], glHeight[i], 0, tFormat[i], GL_UNSIGNED_BYTE, pData[i] );

	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, bFiltered ? GL_LINEAR : GL_NEAREST );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, bFiltered ? GL_LINEAR : GL_NEAREST );
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
	return UTIL_Write( pFile, sName );
}

bool UTIL_Read( CFile *pFile, CCubemapResource *&pData )
{
	char *sName;
	if (!UTIL_Read( pFile, sName ))
		return false;

	pData = pResourceManager->GetCubemapResource( sName );
	delete[] sName;
	return pData != NULL;
}

bool UTIL_GetValue( const CTextItem *pTextItem, CCubemapResource *&pValue )
{
	const char *sName;
	if (!UTIL_GetValue( pTextItem, sName ))
		return false;

	pValue = pResourceManager->GetCubemapResource( sName );
	return pValue != NULL;
}