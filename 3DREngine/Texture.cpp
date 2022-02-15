#include "Texture.h"
#include "AssetManager.h"

CTexture::CTexture( unsigned int uiID, const char *sPath ) : BaseClass( sPath )
{
	m_uiID = uiID;
}

CTexture::~CTexture()
{
	pAssetManager->UnbindTexture( m_uiID );

	glDeleteTextures( 1, &m_uiID );
}

EAssetType CTexture::GetAssetType( void ) const
{
	return EAssetType::t_texture;
}

unsigned int CTexture::GetID( void ) const
{
	return m_uiID;
}