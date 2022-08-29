#ifndef BASETEXTURE_H
#define BASETEXTURE_H

#include "Global.h"
#include "BaseAsset.h"

class CBaseTexture : public CBaseAsset
{
public:
	DECLARE_CLASS( CBaseTexture, CBaseAsset )

	CBaseTexture();

	virtual int Bind( void );

	void SetFiltered( bool bFiltered );

protected:
	bool GetFiltered( void );

private:
	bool m_bFiltered;
};

#endif // BASETEXTURE_H