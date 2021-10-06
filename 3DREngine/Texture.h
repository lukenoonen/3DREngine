#ifndef TEXTURE_H
#define TEXTURE_H

#include "Global.h"
#include "BaseAsset.h"

class CTexture : public CBaseAsset
{
public:
	DECLARE_CLASS( CTexture, CBaseAsset );

	CTexture( unsigned int uiID, const char *sPath );
	virtual ~CTexture();

	unsigned int GetID( void ) const;

private:
	unsigned int m_uiID;
};

#endif // TEXTURE_H