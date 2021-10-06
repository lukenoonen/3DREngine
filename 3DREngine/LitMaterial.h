#ifndef LITMATERIAL_H
#define LITMATERIAL_H

#include "Global.h"
#include "Material.h"
#include "Texture.h"

class CLitMaterial : public CMaterial
{
public:
	DECLARE_CLASS( CLitMaterial, CMaterial );

	CLitMaterial( CTexture *pDiffuse, CTexture *pSpecular, CTexture *pNormal, float flShininess, const glm::vec2 &vecTextureScale, const char *sPath );
	virtual ~CLitMaterial();

	virtual void Use( void );

private:
	CTexture *m_pDiffuse;
	CTexture *m_pSpecular;
	CTexture *m_pNormal;
	glm::vec2 m_vecTextureScale;
	float m_flShininess;
};

#endif // LITMATERIAL_H