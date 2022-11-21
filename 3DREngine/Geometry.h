#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Global.h"
#include "BaseAsset.h"
#include "GeometryResource.h"

class CGeometry : public CBaseAsset
{
public:
	DECLARE_CLASS( CGeometry, CBaseAsset )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CGeometry();

	void Draw( void ) const;

private:
	CGeometryResource *m_pGeometryResource;
};

#endif // GEOMETRY_H