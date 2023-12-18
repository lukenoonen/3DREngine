#ifndef TEXTMATERIAL_H
#define TEXTMATERIAL_H

#include "Global.h"
#include "BaseMaterial.h"
#include "Font.h"

class CTextMaterial : public CBaseMaterial
{
public:
	DECLARE_CLASS( CTextMaterial, CBaseMaterial )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CTextMaterial();

	virtual bool Init( void );

	virtual EShaderType GetShaderType( void );

	virtual void Apply( void );

	CFont *GetFont( void ) const;

private:
	CHandle<CFont> m_hFont;
	glm::vec3 m_vec3Color;
};

#endif // TEXTMATERIAL_H