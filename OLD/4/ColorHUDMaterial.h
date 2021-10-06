#ifndef COLORHUDMATERIAL_H
#define COLORHUDMATERIAL_H

#include "Global.h"
#include "Material.h"

class CColorHUDMaterial : public CMaterial
{
public:
	DECLARE_CLASS( CColorHUDMaterial, CMaterial );

	CColorHUDMaterial( glm::vec3 &vecColor, const char *sPath, unsigned int uiDrawFlags );

	virtual bool Use( void );

	virtual ShaderType_t GetShaderType( void ) const;

private:
	glm::vec3 m_vecColor;
};

#endif // HUDCOLORMATERIAL_H