#include "DirShadowCamera.h"
#include "CommandManager.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "AssetManager.h"

CDirShadowCamera::CDirShadowCamera()
{
	m_flHeight = 16.0f;
	m_flNear = -16.0f;
	m_flFar = 16.0f;
	m_flBlurRadius = 0.0f;
}

void CDirShadowCamera::Init( void )
{
	m_bUpdateProjection = false;
	m_bUpdateBlurScale = false;

	float flWidth = m_flHeight * GetSizeRatio();
	m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
	m_matProjection = glm::ortho( -flWidth, flWidth, -m_flHeight, m_flHeight, m_flNear, m_flFar );
	m_matTotal = m_matProjection * m_matView;

	SetBlurScale( m_flBlurRadius / (m_flHeight * 2.0f) );

	BaseClass::Init();
}

void CDirShadowCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (m_bUpdateProjection)
	{
		float flWidth = m_flHeight * GetSizeRatio();
		m_matProjection = glm::ortho( -flWidth, flWidth, -m_flHeight, m_flHeight, m_flNear, m_flFar );

		bUpdateTotal = true;
	}

	if (PositionUpdated() || RotationUpdated())
	{
		m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );

		bUpdateTotal = true;
	}

	if (bUpdateTotal)
	{
		m_matTotal = m_matProjection * m_matView;
	}

	if (m_bUpdateBlurScale)
	{
		SetBlurScale( m_flBlurRadius / (m_flHeight * 2.0f) );
		m_bUpdateBlurScale = false;
	}

	BaseClass::PostThink();
}

void CDirShadowCamera::Render( void )
{
	pRenderManager->SetViewportSize( GetSize() );
	pRenderManager->SetFrameBuffer( m_uiTextureFBO );

	pRenderManager->SetRenderPass( RENDERPASS_SHADOW_DIR );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matTotal );

	pEntityManager->DrawUnlitEntities();
}

void CDirShadowCamera::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetShadowMapIndex( pAssetManager->BindTexture( m_uiTexture, GL_TEXTURE_2D ) );

	pShaderManager->SetUniformBufferObject( UBO_SHADOW, 0, 0, 1, &m_matTotal );
}

void CDirShadowCamera::SetHeight( float flHeight )
{
	m_flHeight = flHeight * 0.5f;
	m_bUpdateProjection = true;
	m_bUpdateBlurScale = true;
}

void CDirShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	m_bUpdateProjection = true;
}

void CDirShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	m_bUpdateProjection = true;
}

void CDirShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
	m_bUpdateBlurScale = true;
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