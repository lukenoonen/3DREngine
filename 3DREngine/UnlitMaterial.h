#ifndef UNLITMATERIAL_H
#define UNLITMATERIAL_H

#include "Global.h"
#include "BaseMaterial.h"
#include "Texture.h"
#include "BaseCamera.h"

class CUnlitMaterial : public CBaseMaterial
{
public:
	DECLARE_CLASS( CUnlitMaterial, CBaseMaterial )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CUnlitMaterial();

	virtual void PostThink( void );

	virtual EShaderType GetShaderType( void );

	virtual void Apply( void );

private:
	CHandle<CTexture> m_hDiffuse;

	CHandle<CTexture> m_hCamera;
	CHandle<CBaseCamera> m_hTextureCamera;

	glm::vec2 m_vec2TextureScale;
};

#endif // UNLITMATERIAL_H