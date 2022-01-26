#include "PerspectiveWorldCamera.h"
#include "ShaderManager.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "AssetManager.h"

CPerspectiveWorldCamera::CPerspectiveWorldCamera()
{
	m_flFOV = M_PI;
	m_flNear = 0.1f;
	m_flFar = 1000.0f;
}


void CPerspectiveWorldCamera::Init( void )
{
	m_bUpdateProjection = false;

	m_matProjection = glm::perspective( m_flFOV, GetSizeRatio(), m_flNear, m_flFar );
	m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
	m_matTotal = m_matProjection * m_matView;
	m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );
}

void CPerspectiveWorldCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (m_bUpdateProjection)
	{
		m_matProjection = glm::perspective( m_flFOV, GetSizeRatio(), m_flNear, m_flFar );
		m_bUpdateProjection = false;

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
		m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );
	}

	BaseClass::PostThink();
}

void CPerspectiveWorldCamera::Render( void )
{
	const glm::ivec2 &vecSize = GetSize();
	bool bMSAA = GetMSAALevel() != 0;

	pRenderManager->SetViewportSize( vecSize );
	pRenderManager->SetFrameBuffer( bMSAA ? GetMSAAFBO() : m_uiTextureFBO );

	pShaderManager->SetUniformBufferObject( UBO_VIEW, 0, &m_matTotal );
	pShaderManager->SetUniformBufferObject( UBO_VIEW, 1, &m_matTotalLocked );
	pShaderManager->SetUniformBufferObject( UBO_VIEW, 2, &GetPosition() );

	pEntityManager->DrawEntities();

	if (bMSAA)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, GetMSAAFBO() );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_uiTextureFBO );
		glBlitFramebuffer( 0, 0, vecSize.x, vecSize.y, 0, 0, vecSize.x, vecSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}
}

void CPerspectiveWorldCamera::SetFOV( float flFOV )
{
	m_flFOV = flFOV * 0.5f;
	m_bUpdateProjection = true;
}

void CPerspectiveWorldCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	m_bUpdateProjection = true;
}

void CPerspectiveWorldCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	m_bUpdateProjection = true;
}

void CPerspectiveWorldCamera::CreateTextureBuffers( void )
{
	const glm::ivec2 &vecSize = GetSize();

	glGenFramebuffers( 1, &m_uiTextureFBO );
	glGenTextures( 1, &m_uiTexture );

	glBindTexture( GL_TEXTURE_2D, m_uiTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, vecSize.x, vecSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	glBindFramebuffer( GL_FRAMEBUFFER, m_uiTextureFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_uiTexture, 0 );
	// glDrawBuffer( GL_NONE ); // See what this does exactly
	// glReadBuffer( GL_NONE ); // See what this does exactly
}

void CPerspectiveWorldCamera::DestroyTextureBuffers( void )
{
	pAssetManager->UnbindTexture( m_uiTexture );

	glDeleteFramebuffers( 1, &m_uiTextureFBO );
	glDeleteTextures( 1, &m_uiTexture );
}