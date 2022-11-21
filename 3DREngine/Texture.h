#ifndef TEXTURE_H
#define TEXTURE_H

#include "Global.h"
#include "BaseAsset.h"
#include "TextureResource.h"

class CTexture : public CBaseAsset
{
public:
	DECLARE_CLASS( CTexture, CBaseAsset )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CTexture();

	int Bind( void ) const;

private:
	CTextureResource *m_pTextureResource;
};

#endif // TEXTURE_H