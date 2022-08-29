#include "PointLightShadow.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "CommandManager.h"
#include "AssetManager.h"

CConInt ci_r_pntshadowsize( "r_pntshadowsize", 512 );

CPointLightShadow::CPointLightShadow( float flFadeNear, float flFadeFar, float flBlurRadius, float flConstant, float flLinear, float flQuadratic, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( flConstant, flLinear, flQuadratic, vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flFadeNear = flFadeNear;
	m_flFadeFar = flFadeFar;

	m_flBlurScale = flBlurRadius / M_SQRT2;

	m_matProjectionMatrix = glm::perspective( 2.0f * atanf( (1.0f + m_flBlurScale * 2.0f) ), 1.0f, 0.1f, GetMaxRadius() );
	m_matLightSpaceMatricies[0] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecRight, g_vecBack );
	m_matLightSpaceMatricies[1] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecLeft, g_vecBack );
	m_matLightSpaceMatricies[2] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecFront, g_vecUp );
	m_matLightSpaceMatricies[3] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecBack, g_vecDown );
	m_matLightSpaceMatricies[4] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecUp, g_vecBack );
	m_matLightSpaceMatricies[5] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecDown, g_vecBack );

	int iShadowSize = ci_r_pntshadowsize.GetValue();
	pRenderManager->CreateShadowMapFramebuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap, iShadowSize, iShadowSize );
}

CPointLightShadow::~CPointLightShadow()
{
	pRenderManager->DestroyShadowMapFrameBuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap );
}

void CPointLightShadow::PostThink( void )
{
	if (PositionUpdated())
	{
		m_matLightSpaceMatricies[0] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecFront, g_vecUp );
		m_matLightSpaceMatricies[1] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecRight, g_vecUp );
		m_matLightSpaceMatricies[2] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecBack, g_vecUp );
		m_matLightSpaceMatricies[3] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecLeft, g_vecUp );
		m_matLightSpaceMatricies[4] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecUp, g_vecFront );
		m_matLightSpaceMatricies[5] = m_matProjectionMatrix * glm::lookAt( GetPosition(), GetPosition() + g_vecDown, g_vecFront );
	}
}

void CPointLightShadow::CalculateShadows( void )
{
	if (ci_r_pntshadowsize.WasDispatched())
	{
		pAssetManager->UnbindTexture( m_uiShadowMap );

		pRenderManager->DestroyShadowMapFrameBuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap );

		int iShadowSize = ci_r_pntshadowsize.GetValue();
		pRenderManager->CreateShadowMapFramebuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap, iShadowSize * 6, iShadowSize );
	}

	pRenderManager->SetViewportSize( glm::ivec2( ci_r_pntshadowsize.GetValue() ) );

	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_POINT );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOSITION, 0, &GetPosition() );

	int iShadowSize = ci_r_pntshadowsize.GetValue();

	pRenderManager->SetFrameBuffer( m_uiShadowMapFBO );

	for (unsigned int i = 0; i < 6; i++)
	{
		pRenderManager->SetViewportOffset( glm::vec2( iShadowSize * i, 0 ) );
		pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatricies[i] );
		pRenderManager->DrawEntities();
	}

	pRenderManager->SetViewportOffset( g_vec2Zero );
}

void CPointLightShadow::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiShadowMap, GL_TEXTURE_2D ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 6, m_matLightSpaceMatricies );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWBLUR, 0, &m_flBlurScale );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 0, &m_flFadeNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 1, &m_flFadeFar );

	pShaderManager->SetShaderShadow( SHADERSHADOW_TRUE );
}