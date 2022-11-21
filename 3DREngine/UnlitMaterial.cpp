#include "UnlitMaterial.h"

DEFINE_DATADESC( CUnlitMaterial )

	DEFINE_FIELD( LinkedDataField, CHandle<CTexture>, m_hDiffuse, "diffuse", FL_REQUIRED )
	DEFINE_FIELD( DataField, glm::vec2, m_vec2TextureScale, "texturescale", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CUnlitMaterial, material_unlit )

CUnlitMaterial::CUnlitMaterial()
{
	m_vec2TextureScale = g_vec2One;
}

EShaderType CUnlitMaterial::GetShaderType( void )
{
	switch (pRenderManager->GetRenderPass())
	{
	case ERenderPass::t_depth:
		return EShaderType::t_depth;
	case ERenderPass::t_unlit:
		return EShaderType::t_unlit;
	}

	return BaseClass::GetShaderType();
}

void CUnlitMaterial::Apply( void )
{
	BaseClass::Apply();

	pRenderManager->SetUniform( "u_vecTextureScale", m_vec2TextureScale );

	pRenderManager->SetUniform( "u_sDiffuse", m_hDiffuse->Bind() );
}