#include "PointShadowCamera.h"
#include "CommandManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "AssetManager.h"

CPointShadowCamera::CPointShadowCamera( float flZNear, float flZFar, float flFadeNear, float flFadeFar, float flBlurRadius, unsigned int uiBaseSize, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( flFadeNear, flFadeFar, flBlurRadius / M_SQRT2, uiBaseSize, 1.0f, uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flMaxRadius = flZFar;
	m_matProjection = glm::perspective( M_PI * 0.5f, 1.0f, flZNear, flZFar );
	m_matTotal[0] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecLeft, g_vecUp );
	m_matTotal[1] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecRight, g_vecUp );
	m_matTotal[2] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecDown, g_vecBack );
	m_matTotal[3] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecUp, g_vecFront );
	m_matTotal[4] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecBack, g_vecUp );
	m_matTotal[5] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecFront, g_vecUp );

	CreateTextureBuffers();
}

CPointShadowCamera::~CPointShadowCamera()
{
	DestroyTextureBuffers();
}

void CPointShadowCamera::PostThink( void )
{
	if (PositionUpdated())
	{
		m_matTotal[0] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecLeft, g_vecUp );
		m_matTotal[1] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecRight, g_vecUp );
		m_matTotal[2] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecDown, g_vecBack );
		m_matTotal[3] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecUp, g_vecFront );
		m_matTotal[4] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecBack, g_vecUp );
		m_matTotal[5] = m_matProjection * glm::lookAt( GetPosition(), GetPosition() + g_vecFront, g_vecUp );
	}
}

void CPointShadowCamera::Render( void )
{
	pRenderManager->SetViewportSize( GetSize() );
	pRenderManager->SetFrameBuffer( m_uiTextureFBO );

	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_POINT );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 6, m_matTotal );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTMAXDISTANCE, 0, &m_flMaxRadius );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOSITION, 0, &GetPosition() );
	pRenderManager->DrawNonLitEntities();
}

void CPointShadowCamera::CreateTextureBuffers( void )
{
	const glm::ivec2 &vecSize = GetSize();

	glGenFramebuffers( 1, &m_uiTextureFBO );
	glGenTextures( 1, &m_uiTexture );

	glBindTexture( GL_TEXTURE_CUBE_MAP, m_uiTexture );
	for (unsigned int i = 0; i < 6; i++)
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, vecSize.x, vecSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	glBindFramebuffer( GL_FRAMEBUFFER, m_uiTextureFBO );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_uiTexture, 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
}

void CPointShadowCamera::DestroyTextureBuffers( void )
{
	pAssetManager->UnbindTexture( m_uiTexture );

	glDeleteFramebuffers( 1, &m_uiTextureFBO );
	glDeleteTextures( 1, &m_uiTexture );
}

void CPointShadowCamera::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiTexture, GL_TEXTURE_CUBE_MAP ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 6, m_matTotal );
}