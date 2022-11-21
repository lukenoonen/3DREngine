#ifndef SKYBOXMATERIAL_H
#define SKYBOXMATERIAL_H

#include "Global.h"
#include "BaseMaterial.h"
#include "Cubemap.h"

class CSkyboxMaterial : public CBaseMaterial
{
public:
	DECLARE_CLASS( CSkyboxMaterial, CBaseMaterial )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CSkyboxMaterial();

	virtual EShaderType GetShaderType( void );

	virtual void Apply( void );

private:
	CHandle<CCubemap> m_hCubemap;
};

#endif // SKYBOXMATERIAL_H