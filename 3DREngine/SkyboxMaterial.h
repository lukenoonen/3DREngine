#ifndef SKYBOXMATERIAL_H
#define SKYBOXMATERIAL_H

#include "Global.h"
#include "BaseMaterial.h"
#include "CubemapTexture.h"

class CSkyboxMaterial : public CBaseMaterial
{
public:
	DECLARE_CLASS( CSkyboxMaterial, CBaseMaterial )

	CSkyboxMaterial();

	virtual bool Init( void );

	virtual EShaderType GetShaderType( void );

	virtual void Apply( void );

	void SetSkybox( CCubemapTexture *pSkybox );

private:
	CCubemapTexture *m_pSkybox;
};

#endif // SKYBOXMATERIAL_H