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

	CSkybox();

	virtual void Exit( void );

	virtual void PreDraw( void );
	virtual void Draw( void );

	virtual bool ShouldDraw( void ) const;

	void SetMaterial( const char *sMaterialPath );

private:
	CMaterial *m_pMaterial;
	CGeometry *m_pGeometry;
};

#endif // SKYBOX_H