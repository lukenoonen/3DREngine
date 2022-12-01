#include "TextureResource.h"
#include "RenderManager.h"
#include "ResourceManager.h"

CTextureResource::CTextureResource( STextureResourceData &sData, const char *sName ) : BaseClass( sName )
{
	glGenTextures( 1, &m_glID );
	glBindTexture( GL_TEXTURE_2D, m_glID );

	GLint tInternalFormat = GL_RGB8;
	GLenum tFormat = GL_RGB;

	switch (sData.ucChannels)
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

	glTexImage2D( GL_TEXTURE_2D, 0, tInternalFormat, sData.iWidth, sData.iHeight, 0, tFormat, GL_UNSIGNED_BYTE, sData.pData );
	glGenerateMipmap( GL_TEXTURE_2D );

	GLint tTextureWrap = GL_REPEAT;

	switch (sData.eTextureWrap)
	{
	case ETextureWrap::t_repeat:
		tTextureWrap = GL_REPEAT;
		break;
	case ETextureWrap::t_mirrored:
		tTextureWrap = GL_MIRRORED_REPEAT;
		break;
	case ETextureWrap::t_edge:
		tTextureWrap = GL_CLAMP_TO_EDGE;
		break;
	case ETextureWrap::t_border:
		tTextureWrap = GL_CLAMP_TO_BORDER;
		break;
	}

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, tTextureWrap );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, tTextureWrap );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, sData.bFiltered ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, sData.bFiltered ? GL_LINEAR : GL_NEAREST );
	if (sData.eTextureWrap == ETextureWrap::t_border)
		glTexParameterfv( GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, glm::value_ptr( sData.vec4BorderColor ) );
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
	return pFile->Write( sName );
}

bool UTIL_Read( CFile *pFile, CTextureResource *&pData )
{
	char *sName;
	if (!pFile->Read( sName ))
		return false;

	pData = pResourceManager->GetTextureResource( sName );
	delete[] sName;
	return pData != NULL;
}

bool UTIL_GetValue( const CTextItem *pTextItem, CTextureResource *&pValue )
{
	const char *sName;
	if (!pTextItem->GetValue( sName ))
		return false;

	pValue = pResourceManager->GetTextureResource( sName );
	return pValue != NULL;
}