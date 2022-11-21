#ifndef LITMATERIAL_H
#define LITMATERIAL_H

#include "Global.h"
#include "BaseMaterial.h"
#include "Texture.h"

class CLitMaterial : public CBaseMaterial
{
public:
	DECLARE_CLASS( CLitMaterial, CBaseMaterial )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CLitMaterial();

	virtual void PostThink( void );

	virtual EShaderType GetShaderType( void );

	virtual void Apply( void );

private:
	CHandle<CTexture> m_hDiffuse;

	CHandle<CTexture> m_hSpecular;
	float m_flShininess;

	CHandle<CTexture> m_hNormal;

	CHandle<CTexture> m_hCamera;

	glm::vec2 m_vec2TextureScale;

	bool m_bRecieveShadows;
	bool m_bCastShadows;
};

#endif // LITMATERIAL_H