#include "OrthoWorldCamera.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "AssetManager.h"

COrthoWorldCamera::COrthoWorldCamera( float flHeight, float flNear, float flFar, unsigned int uiBaseSize, float flSizeRatio, unsigned int uiBaseMSAALevel, unsigned int uiRenderPriority ) : BaseClass( uiBaseSize, flSizeRatio, uiBaseMSAALevel, uiRenderPriority )
{
	flHeight *= 0.5f;
	float flWidth = flHeight * flSizeRatio;
	m_matProjection = glm::ortho( -flWidth, flWidth, -flHeight, flHeight, flNear, flFar );

	glm::mat4 matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
	m_matTotal = m_matProjection * matView;
	m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( matView ) );

	CreateTextureBuffers();
}

COrthoWorldCamera::~COrthoWorldCamera()
{
	DestroyTextureBuffers();
}

void COrthoWorldCamera::PostThink( void )
{
	if (PositionUpdated() || RotationUpdated())
	{
		glm::mat4 matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vecFront, GetRotation() * g_vecUp );
		m_matTotal = m_matProjection * matView;
		m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( matView ) );
	}
}

void COrthoWorldCamera::Render( void )
{
	const glm::ivec2 &vecSize = GetSize();
	bool bMSAA = GetMSAALevel() != 0;

	pRenderManager->SetViewportSize( vecSize );
	pRenderManager->SetFrameBuffer( bMSAA ? GetMSAAFBO() : m_uiTextureFBO );

	pShaderManager->SetUniformBufferObject( UBO_VIEW, 0, &m_matTotal );
	pShaderManager->SetUniformBufferObject( UBO_VIEW, 1, &m_matTotalLocked );
	pShaderManager->SetUniformBufferObject( UBO_VIEW, 2, &GetPosition() );

	pRenderManager->DrawEntities();

	if (bMSAA)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, GetMSAAFBO() );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_uiTextureFBO );
		glBlitFramebuffer( 0, 0, vecSize.x, vecSize.y, 0, 0, vecSize.x, vecSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}
}

void COrthoWorldCamera::CreateTextureBuffers( void )
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

void COrthoWorldCamera::DestroyTextureBuffers( void )
{
	pAssetManager->UnbindTexture( m_uiTexture );

	glDeleteFramebuffers( 1, &m_uiTextureFBO );
	glDeleteTextures( 1, &m_uiTexture );
}