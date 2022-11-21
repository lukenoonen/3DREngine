#include "Geometry.h"

DEFINE_DATADESC( CGeometry )

	DEFINE_FIELD( DataField, CGeometryResource *, m_pGeometryResource, "geometryresource", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CGeometry, geometry )

CGeometry::CGeometry()
{

}

void CGeometry::Draw( void ) const
{
	m_pGeometryResource->Draw();
}