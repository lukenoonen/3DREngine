#ifndef SKYBOXMATERIAL_H
#define SKYBOXMATERIAL_H

#include "Global.h"
#include "Material.h"
#include "Texture.h"

class CSkyboxMaterial : public CMaterial
{
public:
	DECLARE_CLASS( CSkyboxMaterial, CMaterial );

	CSkyboxMaterial( CTexture *pSkybox, const char *sPath, unsigned int uiDrawFlags );

	virtual bool Use( void );

	virtual ShaderType_t GetShaderType( void ) const;

private:
	CTexture *m_pSkybox;
};

#endif // SKYBOXMATERIAL_H