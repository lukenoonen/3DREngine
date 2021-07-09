#ifndef UNLITMATERIAL_H
#define UNLITMATERIAL_H

#include "Global.h"
#include "Material.h"
#include "Texture.h"

class CUnlitMaterial : public CMaterial
{
public:
	DECLARE_CLASS( CUnlitMaterial, CMaterial );

	CUnlitMaterial( CTexture *pDiffuse, const glm::vec2 &vecTextureScale, const char *sPath, unsigned int uiDrawFlags );

	virtual bool Use( void );

	virtual ShaderType_t GetShaderType( void ) const;

private:
	CTexture *m_pDiffuse;
	glm::vec2 m_vecTextureScale;
};

#endif // UNLITMATERIAL_H