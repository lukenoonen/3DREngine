#include "LitMaterial.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "AssetManager.h"
#include "BaseCamera.h"

CLitMaterial::CLitMaterial( CTexture *pDiffuse, CTexture *pSpecular, float flShininess, CTexture *pNormal, CTexture *pCamera, const glm::vec2 &vec2TextureScale, bool bRecieveShadows, bool bCastShadows, const char *sPath ) : BaseClass( sPath )
{
	m_pDiffuse = pDiffuse;

	m_pSpecular = pSpecular;
	m_flShininess = flShininess;

	m_pNormal = pNormal;

	m_pCamera = pCamera;

	m_vec2TextureScale = vec2TextureScale;

	m_bRecieveShadows = bRecieveShadows;

	m_pDiffuse->Activate();

	if (m_pSpecular)
		m_pSpecular->Activate();

	if (m_pNormal)
		m_pNormal->Activate();

	if (m_pCamera)
		m_pCamera->Activate();

	if (bCastShadows)
	{
		SetShaderType( ERenderPass::t_shadowdir, EShaderType::t_shadowdir );
		SetShaderType( ERenderPass::t_shadowpoint, EShaderType::t_shadowpoint );
		SetShaderType( ERenderPass::t_shadowspot, EShaderType::t_shadowspot );
		SetShaderType( ERenderPass::t_shadowcsm, EShaderType::t_shadowcsm );
	}

	SetShaderType( ERenderPass::t_depth, EShaderType::t_depth );
	SetShaderType( ERenderPass::t_litdir, EShaderType::t_litdir );
	SetShaderType( ERenderPass::t_litpoint, EShaderType::t_litpoint );
	SetShaderType( ERenderPass::t_litspot, EShaderType::t_litspot );
	SetShaderType( ERenderPass::t_litcsm, EShaderType::t_litcsm );
}

CLitMaterial::~CLitMaterial()
{
	m_pDiffuse->Inactivate();
	pAssetManager->CheckTexture( m_pDiffuse );

	if (m_pSpecular)
	{
		m_pSpecular->Inactivate();
		pAssetManager->CheckTexture( m_pSpecular );
	}

	if (m_pNormal)
	{
		m_pNormal->Inactivate();
		pAssetManager->CheckTexture( m_pNormal );
	}

	if (m_pCamera)
	{
		m_pCamera->Inactivate();
		pAssetManager->CheckTexture( m_pCamera );
	}
}

void CLitMaterial::Use( void )
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

		BaseClass::Use();

		pRenderManager->SetUniform( "u_sDiffuse", pAssetManager->BindTexture( m_pDiffuse->GetID(), GL_TEXTURE_2D ) );

		if ((EShaderPreprocessorQuality)pRenderManager->GetShaderPreprocessor( EShaderPreprocessor::t_quality ) != EShaderPreprocessorQuality::t_low) // TODO: make normals not render on low quality in the shader
		{
			if (bDisplaySpecular)
			{
				pRenderManager->SetUniform( "u_sSpecular", pAssetManager->BindTexture( m_pSpecular->GetID(), GL_TEXTURE_2D ) );
				pRenderManager->SetUniform( "u_flShininess", m_flShininess ); // TODO: check shaders to see if this is all aligned
			}

			if (bDisplayNormal)
			{
				pRenderManager->SetUniform( "u_sNormal", pAssetManager->BindTexture( m_pNormal->GetID(), GL_TEXTURE_2D ) );
			}
		}

		if (bDisplayCamera)
		{
			pRenderManager->SetUniform( "u_sCamera", pAssetManager->BindTexture( m_pCamera->GetID(), GL_TEXTURE_2D ) ); // TODO: add these to shaders
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
		BaseClass::Use();
		break;
	}
	}
}