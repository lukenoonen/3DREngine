#include "UnlitMaterial.h"

DEFINE_DATADESC( CUnlitMaterial )

	DEFINE_FIELD( LinkedDataField, CHandle<CTexture>, m_hDiffuse, "diffuse", FL_REQUIRED )
	DEFINE_FIELD( LinkedDataField, CHandle<CTexture>, m_hCamera, "camera", FL_NONE )
	DEFINE_FIELD( LinkedDataField, CHandle<CBaseCamera>, m_hTextureCamera, "texturecamera", FL_NONE )
	DEFINE_FIELD( DataField, glm::vec2, m_vec2TextureScale, "texturescale", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CUnlitMaterial, asset_material_unlit )

CUnlitMaterial::CUnlitMaterial()
{
	m_vec2TextureScale = g_vec2One;
}

void CUnlitMaterial::PostThink( void )
{
	m_hCamera.Check();
	m_hTextureCamera.Check();

	BaseClass::PostThink();
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
	switch (pRenderManager->GetRenderPass())
	{
	case ERenderPass::t_unlit:
	{
		bool bDisplayCamera = m_hTextureCamera != NULL && m_hCamera != NULL;

		BaseClass::Apply();

		pRenderManager->SetUniform( "u_sDiffuse", m_hDiffuse->Bind() );

		pRenderManager->SetUniform( "u_bUseCamera", bDisplayCamera );
		if (bDisplayCamera)
		{
			pRenderManager->SetUniform( "u_sCamera", m_hCamera->Bind() );
			pRenderManager->SetUniform( "u_sCameraTexture", m_hTextureCamera->Bind() );
		}
		break;
	}
	default:
	{
		BaseClass::Apply();
		break;
	}
	}

	pRenderManager->SetUniform( "u_vecTextureScale", m_vec2TextureScale );

}