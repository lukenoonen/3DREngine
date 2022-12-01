#include "LitMaterial.h"
#include "BaseCamera.h"
#include "EntityManager.h"
#include "RenderManager.h"

DEFINE_DATADESC( CLitMaterial )

	DEFINE_FIELD( LinkedDataField, CHandle<CTexture>, m_hDiffuse, "diffuse", FL_REQUIRED )
	DEFINE_FIELD( LinkedDataField, CHandle<CTexture>, m_hSpecular, "specular", 0 )
	DEFINE_FIELD( DataField, float, m_flShininess, "shininess", 0 )
	DEFINE_FIELD( LinkedDataField, CHandle<CTexture>, m_hNormal, "normal", 0 )
	DEFINE_FIELD( LinkedDataField, CHandle<CTexture>, m_hCamera, "camera", 0 )
	DEFINE_FIELD( DataField, glm::vec2, m_vec2TextureScale, "texturescale", 0 )
	DEFINE_FIELD( DataField, bool, m_bRecieveShadows, "recieveshadows", 0 )
	DEFINE_FIELD( DataField, bool, m_bCastShadows, "castshadows", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CLitMaterial, asset_material_lit )

CLitMaterial::CLitMaterial()
{
	m_flShininess = 4.0f;

	m_vec2TextureScale = g_vec2One;

	m_bRecieveShadows = true;
	m_bCastShadows = true;
}

void CLitMaterial::PostThink( void )
{
	m_hSpecular.Check();
	m_hNormal.Check();
	m_hCamera.Check();

	BaseClass::PostThink();
}

EShaderType CLitMaterial::GetShaderType( void )
{
	if (m_bCastShadows)
	{
		switch (pRenderManager->GetRenderPass())
		{
		case ERenderPass::t_shadowdir:
			return EShaderType::t_shadowdir;
		case ERenderPass::t_shadowpoint:
			return EShaderType::t_shadowpoint;
		case ERenderPass::t_shadowspot:
			return EShaderType::t_shadowspot;
		case ERenderPass::t_shadowcsm:
			return EShaderType::t_shadowcsm;
		}
	}

	switch (pRenderManager->GetRenderPass())
	{
	case ERenderPass::t_depth:
		return EShaderType::t_depth;
	case ERenderPass::t_litdir:
		return EShaderType::t_litdir;
	case ERenderPass::t_litpoint:
		return EShaderType::t_litpoint;
	case ERenderPass::t_litspot:
		return EShaderType::t_litspot;
	case ERenderPass::t_litcsm:
		return EShaderType::t_litcsm;
	}

	return BaseClass::GetShaderType();
}

#include <iostream>

void CLitMaterial::Apply( void )
{
	switch (pRenderManager->GetRenderPass())
	{
	case ERenderPass::t_litdir:
	case ERenderPass::t_litpoint:
	case ERenderPass::t_litspot:
	case ERenderPass::t_litcsm:
	{
		bool bDisplaySpecular = m_hSpecular != NULL;

		bool bDisplayNormal = m_hNormal != NULL;

		CBaseCamera *pTextureCamera = pEntityManager->GetTextureCamera();
		bool bDisplayCamera = pTextureCamera != NULL && m_hCamera != NULL;

		CBaseCamera *pShadowCamera = pEntityManager->GetShadowCamera();
		bool bDisplayShadow = m_bRecieveShadows && pShadowCamera != NULL;

		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_specular, bDisplaySpecular ? (EBaseEnum)EShaderPreprocessorSpecular::t_true : (EBaseEnum)EShaderPreprocessorSpecular::t_false );
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_normal, bDisplayNormal ? (EBaseEnum)EShaderPreprocessorNormal::t_true : (EBaseEnum)EShaderPreprocessorNormal::t_false );
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_camera, bDisplayCamera ? (EBaseEnum)EShaderPreprocessorCamera::t_true : (EBaseEnum)EShaderPreprocessorCamera::t_false );
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_shadow, bDisplayShadow ? (EBaseEnum)EShaderPreprocessorShadow::t_true : (EBaseEnum)EShaderPreprocessorShadow::t_false );

		BaseClass::Apply();

		pRenderManager->SetUniform( "u_sDiffuse", m_hDiffuse->Bind() );

		if ((EShaderPreprocessorQuality)pRenderManager->GetShaderPreprocessor( EShaderPreprocessor::t_quality ) != EShaderPreprocessorQuality::t_low) // TODO: make normals not render on low quality in the shader
		{
			if (bDisplaySpecular)
			{
				pRenderManager->SetUniform( "u_sSpecular", m_hSpecular->Bind() );
				pRenderManager->SetUniform( "u_flShininess", m_flShininess ); // TODO: check shaders to see if this is all aligned
			}

			if (bDisplayNormal)
			{
				pRenderManager->SetUniform( "u_sNormal", m_hNormal->Bind() );
			}
		}

		if (bDisplayCamera)
		{
			pRenderManager->SetUniform( "u_sCamera", m_hCamera->Bind() ); // TODO: add these to shaders, verify it works
			pRenderManager->SetUniform( "u_sCameraTexture", pTextureCamera->BindTexture() );
		}

		pRenderManager->SetUniform( "u_vecTextureScale", m_vec2TextureScale );

		if (bDisplayShadow)
		{
			pRenderManager->SetUniform( "u_sShadow", pShadowCamera->BindTexture() );
		}

		break;
	}
	default:
	{
		BaseClass::Apply();
		break;
	}
	}
}