#include "SpotShadowCamera.h"
#include "CommandManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "AssetManager.h"

CSpotShadowCamera::CSpotShadowCamera( float flOuterCutoff, float flZNear, float flZFar, float flFadeNear, float flFadeFar, float flBlurRadius, unsigned int uiBaseSize, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( flFadeNear, flFadeFar, flBlurRadius / (tanf( flOuterCutoff ) * M_SQRT2), uiBaseSize, 1.0f, uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_matProjection = glm::perspective( flOuterCutoff * 2.0f, 1.0f, flZNear, flZFar );
	m_matTotal = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
}

CSpotShadowCamera::~CSpotShadowCamera()
{
	DestroyTextureBuffers();
}

void CSpotShadowCamera::PostThink( void )
{
	if (PositionUpdated() || RotationUpdated())
		m_matTotal = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
}

void CSpotShadowCamera::Render( void )
{
	pRenderManager->SetViewportSize( GetSize() );
	pRenderManager->SetFrameBuffer( m_uiTextureFBO );

	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_SPOT );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matTotal );
	pRenderManager->DrawNonLitEntities();
}

void CSpotShadowCamera::CreateTextureBuffers( void )
{
	const glm::ivec2 &vecSize = GetSize();

	glGenFramebuffers( 1, &m_uiTextureFBO );
	glGenTextures( 1, &m_uiTexture );

	glBindTexture( GL_TEXTURE_2D, m_uiTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, vecSize.x, vecSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	glBindFramebuffer( GL_FRAMEBUFFER, m_uiTextureFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uiTexture, 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
}

void CSpotShadowCamera::DestroyTextureBuffers( void )
{
	pAssetManager->UnbindTexture( m_uiTexture );

	glDeleteFramebuffers( 1, &m_uiTextureFBO );
	glDeleteTextures( 1, &m_uiTexture );
}

void CSpotShadowCamera::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiTexture, GL_TEXTURE_2D ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matTotal );
}