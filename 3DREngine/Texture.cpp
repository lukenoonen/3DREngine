#include "Texture.h"

CTexture::CTexture( unsigned int uiID, const char *sPath ) : BaseClass( sPath )
{
	m_uiID = uiID;
}

CTexture::~CTexture()
{
	glDeleteTextures( 1, &m_uiID );
}

unsigned int CTexture::GetID( void ) const
{
	return m_uiID;
}