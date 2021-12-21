#include "DirShadowCamera.h"
#include "CommandManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "AssetManager.h"

CDirShadowCamera::CDirShadowCamera( float flLength, float flZNear, float flZFar, float flFadeNear, float flFadeFar, float flBlurRadius, unsigned int uiBaseSize, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( flFadeNear, flFadeFar, flBlurRadius / flLength, uiBaseSize, 1.0f, uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	flLength *= 0.5f;
	m_matProjection = glm::ortho( -flLength, flLength, -flLength, flLength, flZNear, flZFar );
	m_matTotal = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );

	CreateTextureBuffers();
}

CDirShadowCamera::~CDirShadowCamera()
{
	DestroyTextureBuffers();
}

void CDirShadowCamera::PostThink( void )
{
	if (PositionUpdated() || RotationUpdated())
		m_matTotal = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
}

void CDirShadowCamera::Render( void )
{
	pRenderManager->SetViewportSize( GetSize() );
	pRenderManager->SetFrameBuffer( m_uiTextureFBO );

	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_DIR );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matTotal );
	pRenderManager->DrawNonLitEntities();
}

void CDirShadowCamera::CreateTextureBuffers( void )
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

void CDirShadowCamera::DestroyTextureBuffers( void )
{
	pAssetManager->UnbindTexture( m_uiTexture );

	glDeleteFramebuffers( 1, &m_uiTextureFBO );
	glDeleteTextures( 1, &m_uiTexture );
}

void CDirShadowCamera::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiTexture, GL_TEXTURE_2D ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matTotal );
}