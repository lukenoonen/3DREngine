#ifndef LITMATERIAL_H
#define LITMATERIAL_H

#include "Global.h"
#include "BaseMaterial.h"
#include "FlatTexture.h"

class CLitMaterial : public CBaseMaterial
{
public:
	DECLARE_CLASS( CLitMaterial, CBaseMaterial )

	CLitMaterial();

	virtual bool Init( void );

	virtual EShaderType GetShaderType( void );

	virtual void Apply( void );

	void SetDiffuse( CFlatTexture *pDiffuse );

	void SetSpecular( CFlatTexture *pSpecular );
	void SetShininess( float flShininess );

	void SetNormal( CFlatTexture *pNormal );

	void SetCamera( CFlatTexture *pCamera );

	void SetTextureScale( const glm::vec2 &vec2TextureScale );

	void SetRecieveShadows( bool bRecieveShadows );
	void SetCastShadows( bool bCastShadows );

private:
	CFlatTexture *m_pDiffuse;

	CFlatTexture *m_pSpecular;
	float m_flShininess;

	CFlatTexture *m_pNormal;

	CFlatTexture *m_pCamera;

	glm::vec2 m_vec2TextureScale;

	bool m_bRecieveShadows;
	bool m_bCastShadows;
};

#endif // LITMATERIAL_H