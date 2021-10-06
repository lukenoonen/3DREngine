#ifndef SKYBOX_H
#define SKYBOX_H

#include "Global.h"
#include "BaseDrawable.h"
#include "Geometry.h"
#include "Material.h"

class CSkybox : public CBaseDrawable
{
public:
	DECLARE_CLASS( CSkybox, CBaseDrawable );

	CSkybox( const char *sMaterialPath, bool bShouldDraw, bool bActive );
	virtual ~CSkybox();

	virtual void Draw( void );

	virtual bool ShouldDraw( void ) const;

private:
	CMaterial *m_pMaterial;
	CGeometry *m_pGeometry;
};

#endif // SKYBOX_H