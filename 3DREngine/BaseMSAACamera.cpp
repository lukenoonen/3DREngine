#include "BaseMSAACamera.h"

CBaseMSAACamera::CBaseMSAACamera()
{
	m_ucMSAALevel = 0;
}

void CBaseMSAACamera::SetMSAALevel( unsigned char ucMSAALevel )
{
	if (m_ucMSAALevel != ucMSAALevel)
	{
		m_ucMSAALevel = ucMSAALevel;

		SetUpdateMSAABuffers( true );
	}
}

unsigned char CBaseMSAACamera::GetMSAALevel( void ) const
{
	return m_ucMSAALevel;
}

GLuint CBaseMSAACamera::GetMSAAFBO( void ) const
{
	return m_glMSAAFBO;
}

void CBaseMSAACamera::CreateMSAABuffers( void )
{
	if (m_ucMSAALevel != 0)
	{
		const glm::ivec2 &vec2Size = GetSize();

		glGenFramebuffers( 1, &m_glMSAAFBO );
		glGenTextures( 1, &m_glMSAA );
		glGenRenderbuffers( 1, &m_glMSAARBO );

		glBindFramebuffer( GL_FRAMEBUFFER, m_glMSAAFBO );
		glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_glMSAA );
		glBindRenderbuffer( GL_RENDERBUFFER, m_glMSAARBO );

		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, (GLsizei)m_ucMSAALevel, GL_RGB8, vec2Size.x, vec2Size.y, GL_TRUE );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_glMSAA, 0 );
		glRenderbufferStorageMultisample( GL_RENDERBUFFER, (GLsizei)m_ucMSAALevel, GL_DEPTH24_STENCIL8, vec2Size.x, vec2Size.y );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_glMSAARBO );
	}
}

void CBaseMSAACamera::DestroyMSAABuffers( void )
{
	if (m_ucMSAALevel != 0)
	{
		glDeleteFramebuffers( 1, &m_glMSAAFBO );
		glDeleteTextures( 1, &m_glMSAA );
		glDeleteRenderbuffers( 1, &m_glMSAARBO );
	}
}