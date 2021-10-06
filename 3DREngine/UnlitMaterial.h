#ifndef UNLITMATERIAL_H
#define UNLITMATERIAL_H

#include "Global.h"
#include "Material.h"
#include "Texture.h"

class CUnlitMaterial : public CMaterial
{
public:
	DECLARE_CLASS( CUnlitMaterial, CMaterial );

	CUnlitMaterial( CTexture *pDiffuse, const glm::vec2 &vecTextureScale, const char *sPath );
	virtual ~CUnlitMaterial();

	virtual void Use( void );

private:
	CTexture *m_pDiffuse;
	glm::vec2 m_vecTextureScale;
};

#endif // UNLITMATERIAL_H