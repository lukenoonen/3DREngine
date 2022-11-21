#ifndef CUBEMAP_H
#define CUBEMAP_H

#include "Global.h"
#include "BaseAsset.h"
#include "CubemapResource.h"

class CCubemap : public CBaseAsset
{
public:
	DECLARE_CLASS( CCubemap, CBaseAsset )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CCubemap();

	int Bind( void ) const;

private:
	CCubemapResource *m_pCubemapResource;
};

#endif // CUBEMAP_H