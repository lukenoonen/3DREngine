#ifndef SKYBOX_H
#define SKYBOX_H

#include "Global.h"
#include "BaseDrawable.h"
#include "SkyboxMaterial.h"
#include "Geometry.h"

class CSkybox : public CBaseDrawable
{
public:
	DECLARE_CLASS( CSkybox, CBaseDrawable )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CSkybox();

	virtual void PreDraw( void );
	virtual void Draw( void );

	virtual bool ShouldDraw( void ) const;

private:
	CHandle<CSkyboxMaterial> m_hSkyboxMaterial;
	CHandle<CGeometry> m_hGeometry;
};

#endif // SKYBOX_H