#include "LitMaterial.h"
#include "BaseCamera.h"
#include "EntityManager.h"
#include "RenderManager.h"

CLitMaterial::CLitMaterial()
{
	m_pDiffuse = NULL;

	m_pSpecular = NULL;
	m_flShininess;

	m_pNormal = NULL;

	m_pCamera = NULL;

	m_vec2TextureScale = g_vec2One;

	m_bRecieveShadows = false;
	m_bCastShadows = false;
}

bool CLitMaterial::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (!m_pDiffuse)
		return false;

	return true;
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

void CLitMaterial::Apply( void )
{
	switch (pRenderManager->GetRenderPass())
	{
	case ERenderPass::t_litdir:
	case ERenderPass::t_litpoint:
	case ERenderPass::t_litspot:
	case ERenderPass::t_litcsm:
	{
		bool bDisplaySpecular = m_pSpecular != NULL;

		bool bDisplayNormal = m_pNormal != NULL;

		CBaseCamera *pTextureCamera = pEntityManager->GetTextureCamera();
		bool bDisplayCamera = pTextureCamera != NULL && m_pCamera != NULL;

		CBaseCamera *pShadowCamera = pEntityManager->GetShadowCamera();
		bool bDisplayShadow = m_bRecieveShadows && pShadowCamera != NULL;

		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_specular, bDisplaySpecular ? (EBaseEnum)EShaderPreprocessorSpecular::t_true : (EBaseEnum)EShaderPreprocessorSpecular::t_false );
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_normal, bDisplayNormal ? (EBaseEnum)EShaderPreprocessorNormal::t_true : (EBaseEnum)EShaderPreprocessorNormal::t_false );
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_camera, bDisplayCamera ? (EBaseEnum)EShaderPreprocessorCamera::t_true : (EBaseEnum)EShaderPreprocessorCamera::t_false );
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_shadow, bDisplayShadow ? (EBaseEnum)EShaderPreprocessorShadow::t_true : (EBaseEnum)EShaderPreprocessorShadow::t_false );

		BaseClass::Apply();

		pRenderManager->SetUniform( "u_sDiffuse", m_pDiffuse->Bind() );

		if ((EShaderPreprocessorQuality)pRenderManager->GetShaderPreprocessor( EShaderPreprocessor::t_quality ) != EShaderPreprocessorQuality::t_low) // TODO: make normals not render on low quality in the shader
		{
			if (bDisplaySpecular)
			{
				pRenderManager->SetUniform( "u_sSpecular", m_pSpecular->Bind() );
				pRenderManager->SetUniform( "u_flShininess", m_flShininess ); // TODO: check shaders to see if this is all aligned
			}

			if (bDisplayNormal)
			{
				pRenderManager->SetUniform( "u_sNormal", m_pNormal->Bind() );
			}
		}

		if (bDisplayCamera)
		{
			pRenderManager->SetUniform( "u_sCamera", m_pCamera->Bind() ); // TODO: add these to shaders
			pRenderManager->SetUniform( "u_sCameraTexture", pTextureCamera->BindTexture() );
		}

		pRenderManager->SetUniform( "u_vecTextureScale", m_vec2TextureScale );

		if (bDisplayShadow)
		{
			pRenderManager->SetUniform( "u_sShadow", pShadowCamera->BindTexture() ); // TODO: make this u_sShadow instead of u_sShadowMap in the shaders
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

void CLitMaterial::SetDiffuse( CFlatTexture *pDiffuse )
{
	m_pDiffuse = pDiffuse;
}

void CLitMaterial::SetSpecular( CFlatTexture *pSpecular )
{
	m_pSpecular = pSpecular;
}

void CLitMaterial::SetShininess( float flShininess )
{
	m_flShininess = flShininess;
}

void CLitMaterial::SetNormal( CFlatTexture *pNormal )
{
	m_pNormal = pNormal;
}

void CLitMaterial::SetCamera( CFlatTexture *pCamera )
{
	m_pCamera = pCamera;
}

void CLitMaterial::SetTextureScale( const glm::vec2 &vec2TextureScale )
{
	m_vec2TextureScale = vec2TextureScale;
}

void CLitMaterial::SetRecieveShadows( bool bRecieveShadows )
{
	m_bRecieveShadows = bRecieveShadows;
}

void CLitMaterial::SetCastShadows( bool bCastShadows )
{
	m_bCastShadows = bCastShadows;
}