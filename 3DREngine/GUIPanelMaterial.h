#ifndef GUIPANELMATERIAL_H
#define GUIPANELMATERIAL_H

#include "Global.h"
#include "BaseMaterial.h"

class CGUIPanelMaterial : public CBaseMaterial
{
public:
	DECLARE_CLASS( CGUIPanelMaterial, CBaseMaterial )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CGUIPanelMaterial();

	virtual EShaderType GetShaderType( void );

	virtual void Apply( void );

private:
	glm::vec3 m_vec3Color;
};

#endif // GUIPANELMATERIAL_H