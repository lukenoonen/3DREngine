#include "TextureResource.h"
#include "RenderManager.h"
#include "ResourceManager.h"

CTextureResource::CTextureResource( unsigned char *pData, GLsizei glWidth, GLsizei glHeight, GLenum tInternalFormat, GLenum tFormat, bool bFiltered, GLint tTextureWrap, glm::vec4 vec4BorderColor, const char *sName ) : BaseClass( sName )
{
	glGenTextures( 1, &m_glID );
	glBindTexture( GL_TEXTURE_2D, m_glID );
	glTexImage2D( GL_TEXTURE_2D, 0, tInternalFormat, glWidth, glHeight, 0, tFormat, GL_UNSIGNED_BYTE, pData );
	glGenerateMipmap( GL_TEXTURE_2D );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tTextureWrap );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tTextureWrap );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, bFiltered ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, bFiltered ? GL_LINEAR : GL_NEAREST );
	if (tTextureWrap == GL_CLAMP_TO_BORDER)
		glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr( vec4BorderColor ) );
}

CTextureResource::~CTextureResource()
{
	pRenderManager->UnbindTexture( m_glID );
	glDeleteTextures( 1, &m_glID );
}

int CTextureResource::Bind( void ) const
{
	return pRenderManager->BindTexture( m_glID, GL_TEXTURE_2D );
}

bool UTIL_Write( CFile *pFile, CTextureResource *&pData )
{
	const char *sName = pData->GetName();
	return UTIL_Write( pFile, sName );
}

bool UTIL_Read( CFile *pFile, CTextureResource *&pData )
{
	char *sName;
	if (!UTIL_Read( pFile, sName ))
		return false;

	pData = pResourceManager->GetTextureResource( sName );
	delete[] sName;
	return pData != NULL;
}

bool UTIL_GetValue( const CTextItem *pTextItem, CTextureResource *&pValue )
{
	const char *sName;
	if (!UTIL_GetValue( pTextItem, sName ))
		return false;

	pValue = pResourceManager->GetTextureResource( sName );
	return pValue != NULL;
}