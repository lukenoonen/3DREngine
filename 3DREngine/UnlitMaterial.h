#ifndef UNLITMATERIAL_H
#define UNLITMATERIAL_H

#include "Global.h"
#include "BaseMaterial.h"
#include "FlatTexture.h"

class CUnlitMaterial : public CBaseMaterial
{
public:
	DECLARE_CLASS( CUnlitMaterial, CBaseMaterial )

	CUnlitMaterial();

	virtual bool Init( void );

	virtual EShaderType GetShaderType( void );

	virtual void Apply( void );

	void SetDiffuse( CFlatTexture *pDiffuse );

	void SetTextureScale( const glm::vec2 &vec2TextureScale );

private:
	CFlatTexture *m_pDiffuse;

	glm::vec2 m_vec2TextureScale;
};

#endif // UNLITMATERIAL_H