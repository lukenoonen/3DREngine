#ifndef MESH_H
#define MESH_H

#include "Global.h"
#include "BaseAsset.h"
#include "Geometry.h"
#include "Material.h"

class CMesh : public CBaseAsset
{
public:
	DECLARE_CLASS( CMesh, CBaseAsset );

	CMesh( CGeometry *pGeometry, CMaterial *pMaterial, const char *sPath );
	virtual ~CMesh();

	virtual bool ShouldDraw( void ) const;

	void Draw( void );

private:
	CGeometry *m_pGeometry;
	CMaterial *m_pMaterial;
};

#endif // MESH_H