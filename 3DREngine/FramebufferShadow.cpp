#include "FramebufferShadow.h"
#include "RenderManager.h"

DEFINE_DATADESC_NOBASE( CFramebufferShadow )

	DEFINE_FIELD( DataField, glm::ivec2, m_vec2Size, "size", 0 )

END_DATADESC()

CFramebufferShadow::CFramebufferShadow()
{
	m_vec2Size = g_ivec2One * 1024;
}

const glm::ivec2 &CFramebufferShadow::GetSize( void ) const
{
	return m_vec2Size;
}

GLuint CFramebufferShadow::GetFramebuffer( void ) const
{
	return m_glTextureFBO;
}

void CFramebufferShadow::CreateTextureBuffers( void )
{
	glGenFramebuffers( 1, &m_glTextureFBO );
	glGenTextures( 1, &m_glTexture );

	glBindTexture( GL_TEXTURE_2D, m_glTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_vec2Size.x, m_vec2Size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	glBindFramebuffer( GL_FRAMEBUFFER, m_glTextureFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_glTexture, 0 );
	glDrawBuffer( GL_NONE ); // TODO: what does this do?
	glReadBuffer( GL_NONE ); // TODO: what does this do?
}

void CFramebufferShadow::DestroyTextureBuffers( void )
{
	pRenderManager->UnbindTexture( m_glTexture );

	glDeleteFramebuffers( 1, &m_glTextureFBO );
	glDeleteTextures( 1, &m_glTexture );
}