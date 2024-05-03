#include "TextMaterial.h"
#include "EntityManager.h"
#include "RenderManager.h"

DEFINE_DATADESC( CTextMaterial )

	DEFINE_FIELD( LinkedDataField, CHandle<CFont>, m_hFont, "font", FL_REQUIRED )
	DEFINE_FIELD( DataField, glm::vec3, m_vec3Color, "color", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CTextMaterial, asset_material_text )

CTextMaterial::CTextMaterial()
{
	m_vec3Color = g_vec3One;
}

bool CTextMaterial::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (!m_hFont)
		return false;

	return true;
}

EShaderType CTextMaterial::GetShaderType( void )
{
	switch (pRenderManager->GetRenderPass())
	{
	case ERenderPass::t_unlit:
		return EShaderType::t_guitext;
	}

	return BaseClass::GetShaderType();
}

void CTextMaterial::Apply( void )
{
	BaseClass::Apply();

	pRenderManager->SetUniform( "u_sFont", m_hFont->BindBitmap() );
	pRenderManager->SetUniform( "u_vecColor", m_vec3Color );
}

CFont *CTextMaterial::GetFont( void ) const
{
	return m_hFont;
}