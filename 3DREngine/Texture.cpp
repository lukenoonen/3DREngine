#include "Texture.h"

DEFINE_DATADESC( CTexture )

	DEFINE_FIELD( DataField, CTextureResource *, m_pTextureResource, "textureresource", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CTexture, texture )

CTexture::CTexture()
{

}

int CTexture::Bind( void ) const
{
	return m_pTextureResource->Bind();
}