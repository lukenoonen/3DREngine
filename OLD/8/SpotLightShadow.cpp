#include "SpotLightShadow.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "CommandManager.h"
#include "AssetManager.h"

CConInt ci_r_sptshadowsize( "r_sptshadowsize", 512 );

CSpotLightShadow::CSpotLightShadow( float flFadeNear, float flFadeFar, float flBlurRadius, float flCutoff, float flOuterCutoff, float flConstant, float flLinear, float flQuadratic, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( flCutoff, flOuterCutoff, flConstant, flLinear, flQuadratic, vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flFadeNear = flFadeNear;
	m_flFadeFar = flFadeFar;

	m_flBlurScale = flBlurRadius / (tanf( flOuterCutoff ) * M_SQRT2);

	m_matProjectionMatrix = glm::perspective( flOuterCutoff * 2.0f, 1.0f, 0.1f, GetMaxRadius() );
	m_matLightSpaceMatrix = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );

	int iShadowSize = ci_r_sptshadowsize.GetValue();
	pRenderManager->CreateShadowMapFramebuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap, iShadowSize, iShadowSize );
}

CSpotLightShadow::~CSpotLightShadow()
{
	pRenderManager->DestroyShadowMapFrameBuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap );
}

void CSpotLightShadow::PostThink( void )
{
	if (PositionUpdated() || RotationUpdated())
	{
		m_matLightSpaceMatrix = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
	}
}

void CSpotLightShadow::CalculateShadows( void )
{
	if (ci_r_sptshadowsize.WasDispatched())
	{
		pAssetManager->UnbindTexture( m_uiShadowMap );

		pRenderManager->DestroyShadowMapFrameBuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap );

		int iShadowSize = ci_r_sptshadowsize.GetValue();
		pRenderManager->CreateShadowMapFramebuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap, iShadowSize, iShadowSize );
	}

	float flMaxRadius = GetMaxRadius();

	pRenderManager->SetViewportSize( glm::ivec2( ci_r_sptshadowsize.GetValue() ) );

	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_SPOT );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatrix );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOSITION, 0, &GetPosition() );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTMAXDISTANCE, 0, &flMaxRadius );

	pRenderManager->DrawShadowMap( m_uiShadowMapFBO );
}

void CSpotLightShadow::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiShadowMap, GL_TEXTURE_2D ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatrix );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWBLUR, 0, &m_flBlurScale );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 0, &m_flFadeNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 1, &m_flFadeFar );

	pShaderManager->SetShaderShadow( SHADERSHADOW_TRUE );
}