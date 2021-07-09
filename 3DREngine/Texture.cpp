#include "Texture.h"
#include <UTIL.h>
#include <glad/glad.h>

CTexture::CTexture( unsigned int uiID, const char *sPath )
{
	m_uiID = uiID;
	m_sPath = UTIL_stredit( sPath );
}

CTexture::~CTexture()
{
	glDeleteTextures( 1, &m_uiID );
	delete[] m_sPath;
}

unsigned int CTexture::GetID( void )
{
	return m_uiID;
}

const char *CTexture::GetPath( void )
{
	return m_sPath;
}