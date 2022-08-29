#include "OrthoWorldCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

COrthoWorldCamera::COrthoWorldCamera()
{
	m_flHeight = 16.0f;
	m_flNear = -16.0f;
	m_flFar = 16.0f;
}

bool COrthoWorldCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateProjection = false;

	float flWidth = m_flHeight * GetSizeRatio();
	m_matProjection = glm::ortho( -flWidth, flWidth, -m_flHeight, m_flHeight, m_flNear, m_flFar );
	m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vec3Front, GetRotation() * g_vec3Up );
	m_matTotal = m_matProjection * m_matView;
	m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );

	return true;
}

void COrthoWorldCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (m_bUpdateProjection)
	{
		float flWidth = m_flHeight * GetSizeRatio();
		m_matProjection = glm::ortho( -flWidth, flWidth, -m_flHeight, m_flHeight, m_flNear, m_flFar );
		m_bUpdateProjection = false;

		bUpdateTotal = true;
	}

	if (PositionUpdated() || RotationUpdated())
	{
		m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vec3Front, GetRotation() * g_vec3Up );

		bUpdateTotal = true;
	}

	if (bUpdateTotal)
	{
		m_matTotal = m_matProjection * m_matView;
		m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );
	}

	BaseClass::PostThink();
}

void COrthoWorldCamera::Render( void )
{
	pEntityManager->SetTextureCamera( this );

	const glm::ivec2 &vec2Size = GetSize();
	bool bMSAA = GetMSAALevel() != 0;

	pRenderManager->SetViewportSize( vec2Size );
	pRenderManager->SetFrameBuffer( bMSAA ? GetMSAAFBO() : m_glTextureFBO );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 0, &m_matTotal );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 1, &m_matTotalLocked );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 2, &GetPosition() );

	pEntityManager->DrawEntities();

	if (bMSAA)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, GetMSAAFBO() );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_glTextureFBO );
		glBlitFramebuffer( 0, 0, vec2Size.x, vec2Size.y, 0, 0, vec2Size.x, vec2Size.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}

	pEntityManager->SetTextureCamera( this );
}

int COrthoWorldCamera::BindTexture( void )
{
	return pRenderManager->BindTexture( m_glTexture, GL_TEXTURE_2D );
}

void COrthoWorldCamera::SetHeight( float flHeight )
{
	m_flHeight = flHeight * 0.5f;
	m_bUpdateProjection = true;
}

void COrthoWorldCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	m_bUpdateProjection = true;
}

void COrthoWorldCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	m_bUpdateProjection = true;
}

void COrthoWorldCamera::CreateTextureBuffers( void )
{
	const glm::ivec2 &vec2Size = GetSize();

	glGenFramebuffers( 1, &m_glTextureFBO );
	glGenTextures( 1, &m_glTexture );

	glBindTexture( GL_TEXTURE_2D, m_glTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, vec2Size.x, vec2Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );
	glBindFramebuffer( GL_FRAMEBUFFER, m_glTextureFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glTexture, 0 );
}

void COrthoWorldCamera::DestroyTextureBuffers( void )
{
	pRenderManager->UnbindTexture( m_glTexture );

	glDeleteFramebuffers( 1, &m_glTextureFBO );
	glDeleteTextures( 1, &m_glTexture );
}