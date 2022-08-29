#include "DirLightShadow.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "CommandManager.h"
#include "AssetManager.h"

CConInt ci_r_dirshadowsize( "r_dirshadowsize", 1024 );

CDirLightShadow::CDirLightShadow( float flFadeNear, float flFadeFar, float flBlurRadius, float flLength, float flZNear, float flZFar, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flFadeNear = flFadeNear;
	m_flFadeFar = flFadeFar;

	m_flBlurScale = flBlurRadius / flLength;

	flLength *= 0.5f;
	m_matOrthoMatrix = glm::ortho( -flLength, flLength, -flLength, flLength, flZNear, flZFar );
	m_matLightSpaceMatrix = m_matOrthoMatrix * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );

	int iShadowSize = ci_r_dirshadowsize.GetValue();
	pRenderManager->CreateShadowMapFramebuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap, iShadowSize, iShadowSize );
}

CDirLightShadow::~CDirLightShadow()
{
	pRenderManager->DestroyShadowMapFrameBuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap );
}

void CDirLightShadow::PostThink( void )
{
	if (PositionUpdated() || RotationUpdated())
	{
		m_matLightSpaceMatrix = m_matOrthoMatrix * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
	}
}

void CDirLightShadow::CalculateShadows( void )
{
	if (ci_r_dirshadowsize.WasDispatched())
	{
		pAssetManager->UnbindTexture( m_uiShadowMap );

		pRenderManager->DestroyShadowMapFrameBuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap );

		int iShadowSize = ci_r_dirshadowsize.GetValue();
		pRenderManager->CreateShadowMapFramebuffer( 1, &m_uiShadowMapFBO, &m_uiShadowMap, iShadowSize, iShadowSize );
	}

	pRenderManager->SetViewportSize( glm::ivec2( ci_r_dirshadowsize.GetValue() ) );

	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_DIR );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatrix );

	pRenderManager->DrawShadowMap( m_uiShadowMapFBO );
}

void CDirLightShadow::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiShadowMap, GL_TEXTURE_2D ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matLightSpaceMatrix );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWBLUR, 0, &m_flBlurScale );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 0, &m_flFadeNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 1, &m_flFadeFar );

	pShaderManager->SetShaderShadow( SHADERSHADOW_TRUE );
}