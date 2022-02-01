#include "BaseMSAACamera.h"

CBaseMSAACamera::CBaseMSAACamera()
{
	m_uiMSAALevel = 0;
}

void CBaseMSAACamera::SetMSAALevel( unsigned int uiMSAALevel )
{
	if (m_uiMSAALevel != uiMSAALevel)
	{
		m_uiMSAALevel = uiMSAALevel;

		SetUpdateMSAABuffers( true );
	}
}

unsigned int CBaseMSAACamera::GetMSAALevel( void ) const
{
	return m_uiMSAALevel;
}

unsigned int CBaseMSAACamera::GetMSAAFBO( void ) const
{
	return m_uiMSAAFBO;
}

void CBaseMSAACamera::CreateMSAABuffers( void )
{
	if (m_uiMSAALevel != 0)
	{
		const glm::ivec2 &vec2Size = GetSize();

		glGenFramebuffers( 1, &m_uiMSAAFBO );
		glGenTextures( 1, &m_uiMSAA );
		glGenRenderbuffers( 1, &m_uiMSAARBO );

		glBindFramebuffer( GL_FRAMEBUFFER, m_uiMSAAFBO );
		glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_uiMSAA );
		glBindRenderbuffer( GL_RENDERBUFFER, m_uiMSAARBO );

		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, m_uiMSAALevel, GL_RGB8, vec2Size.x, vec2Size.y, GL_TRUE );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_uiMSAA, 0 );
		glRenderbufferStorageMultisample( GL_RENDERBUFFER, m_uiMSAALevel, GL_DEPTH24_STENCIL8, vec2Size.x, vec2Size.y );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uiMSAARBO );
	}
}

void CBaseMSAACamera::DestroyMSAABuffers( void )
{
	if (m_uiMSAALevel != 0)
	{
		glDeleteFramebuffers( 1, &m_uiMSAAFBO );
		glDeleteTextures( 1, &m_uiMSAA );
		glDeleteRenderbuffers( 1, &m_uiMSAARBO );
	}
}