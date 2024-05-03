#include "GUIPanelMaterial.h"
#include "EntityManager.h"
#include "RenderManager.h"

DEFINE_DATADESC( CGUIPanelMaterial )

	DEFINE_FIELD( DataField, glm::vec3, m_vec3Color, "color", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CGUIPanelMaterial, asset_material_panel_gui )

CGUIPanelMaterial::CGUIPanelMaterial()
{
	m_vec3Color = g_vec3One;
}

EShaderType CGUIPanelMaterial::GetShaderType( void )
{
	switch (pRenderManager->GetRenderPass())
	{
	case ERenderPass::t_unlit:
		return EShaderType::t_guipanel;
	}

	return BaseClass::GetShaderType();
}

void CGUIPanelMaterial::Apply( void )
{
	BaseClass::Apply();

	pRenderManager->SetUniform( "u_vecColor", m_vec3Color );
}