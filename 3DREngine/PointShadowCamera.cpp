#include "PointShadowCamera.h"
#include "CommandManager.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "AssetManager.h"

CPointShadowCamera::CPointShadowCamera()
{
	m_flNear = 0.1f;
	m_flFar = 1000.0f;

	SetSizeRatio( 1.0f );
}

void CPointShadowCamera::Init( void )
{
	m_bUpdateProjection = false;

	m_matProjection = glm::perspective( M_PI * 0.5f, 1.0f, m_flNear, m_flFar );
	m_matView[0] = glm::lookAt( GetPosition(), GetPosition() + g_vecLeft, g_vecUp );
	m_matView[1] = glm::lookAt( GetPosition(), GetPosition() + g_vecRight, g_vecUp );
	m_matView[2] = glm::lookAt( GetPosition(), GetPosition() + g_vecDown, g_vecBack );
	m_matView[3] = glm::lookAt( GetPosition(), GetPosition() + g_vecUp, g_vecFront );
	m_matView[4] = glm::lookAt( GetPosition(), GetPosition() + g_vecBack, g_vecUp );
	m_matView[5] = glm::lookAt( GetPosition(), GetPosition() + g_vecFront, g_vecUp );
	for (unsigned int i = 0; i < 6; i++)
		m_matTotal[i] = m_matProjection * m_matView[i];

	BaseClass::Init();
}

void CPointShadowCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (m_bUpdateProjection)
	{
		m_matProjection = glm::perspective( M_PI * 0.5f, 1.0f, m_flNear, m_flFar );
		m_bUpdateProjection = false;

		bUpdateTotal = true;
	}

	if (PositionUpdated())
	{
		m_matView[0] = glm::lookAt( GetPosition(), GetPosition() + g_vecLeft, g_vecUp );
		m_matView[1] = glm::lookAt( GetPosition(), GetPosition() + g_vecRight, g_vecUp );
		m_matView[2] = glm::lookAt( GetPosition(), GetPosition() + g_vecDown, g_vecBack );
		m_matView[3] = glm::lookAt( GetPosition(), GetPosition() + g_vecUp, g_vecFront );
		m_matView[4] = glm::lookAt( GetPosition(), GetPosition() + g_vecBack, g_vecUp );
		m_matView[5] = glm::lookAt( GetPosition(), GetPosition() + g_vecFront, g_vecUp );

		bUpdateTotal = true;
	}

	if (bUpdateTotal)
	{
		for (unsigned int i = 0; i < 6; i++)
			m_matTotal[i] = m_matProjection * m_matView[i];
	}

	BaseClass::PostThink();
}

void CPointShadowCamera::Render( void )
{
	pRenderManager->SetViewportSize( GetSize() );
	pRenderManager->SetFrameBuffer( m_uiTextureFBO );

	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_POINT );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 6, m_matTotal );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTMAXDISTANCE, 0, &m_flFar );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOSITION, 0, &GetPosition() );

	pEntityManager->DrawUnlitEntities();
}

void CPointShadowCamera::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiTexture, GL_TEXTURE_CUBE_MAP ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 6, m_matTotal );
}

void CPointShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	m_bUpdateProjection = true;
}

void CPointShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	m_bUpdateProjection = true;
}

void CPointShadowCamera::SetBlurRadius( float flBlurRadius )
{
	SetBlurScale( flBlurRadius / M_SQRT2 );
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