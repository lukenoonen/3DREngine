#ifndef LITMATERIAL_H
#define LITMATERIAL_H

#include "Global.h"
#include "Material.h"
#include "Texture.h"

class CLitMaterial : public CMaterial
{
public:
	DECLARE_CLASS( CLitMaterial, CMaterial );

	CLitMaterial( CTexture *pDiffuse, CTexture *pSpecular, float flShininess, CTexture *pNormal, CTexture *pCamera, const glm::vec2 &vec2TextureScale, bool bCastShadows, bool bRecieveShadows, const char *sPath );
	virtual ~CLitMaterial();

	virtual void Use( void );

private:
	CTexture *m_pDiffuse;

	CTexture *m_pSpecular;
	float m_flShininess;

	CTexture *m_pNormal;

	CTexture *m_pCamera;

	glm::vec2 m_vec2TextureScale;

	bool m_bRecieveShadows;
};

#endif // LITMATERIAL_H