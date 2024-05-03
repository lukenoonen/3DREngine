#include "FramebufferDefault.h"
#include "RenderManager.h"

CFramebufferDefault::CFramebufferDefault()
{
	m_vec2Size = g_ivec2Zero;
	m_glMSAA = 0;
}

void CFramebufferDefault::Init( void )
{
	m_vec2Size = cv_r_windowsize.GetValue();
	m_ucMSAALevel = ci_r_msaalevel.GetValue();

	BaseClass::Init();
}

void CFramebufferDefault::Think( void )
{
	if (cv_r_windowsize.WasDispatched())
		SetSize( cv_r_windowsize.GetValue() );

	if (ci_r_msaalevel.WasDispatched())
		SetMSAALevel( ci_r_msaalevel.GetValue() );
}

GLuint CFramebufferDefault::GetFramebuffer( void ) const
{
	return m_glMSAAFBO != 0 ? m_glMSAAFBO : 0;
}

void CFramebufferDefault::Blit( void )
{
	if (m_glMSAAFBO != 0)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, m_glMSAAFBO );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glBlitFramebuffer( 0, 0, m_vec2Size.x, m_vec2Size.y, 0, 0, m_vec2Size.x, m_vec2Size.y, GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST );
	}
}

const glm::ivec2 &CFramebufferDefault::GetSize( void ) const
{
	return m_vec2Size;
}

unsigned char CFramebufferDefault::GetMSAALevel( void ) const
{
	return m_ucMSAALevel;
}

bool CFramebufferDefault::SetSizeInternal( const glm::ivec2 &vec2Size )
{
	if (m_vec2Size != vec2Size)
	{
		m_vec2Size = vec2Size;
		return true;
	}

	return false;
}

bool CFramebufferDefault::SetMSAALevelInternal( unsigned char ucMSAALevel )
{
	if (m_ucMSAALevel != ucMSAALevel)
	{
		m_ucMSAALevel = ucMSAALevel;
		return true;
	}

	return false;
}

void CFramebufferDefault::CreateMSAABuffers( void )
{
	if (m_ucMSAALevel != 0)
	{
		glGenFramebuffers( 1, &m_glMSAAFBO );
		glGenTextures( 1, &m_glMSAA );
		glGenRenderbuffers( 1, &m_glMSAARBO );

		glBindFramebuffer( GL_FRAMEBUFFER, m_glMSAAFBO );
		glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_glMSAA );
		glBindRenderbuffer( GL_RENDERBUFFER, m_glMSAARBO );

		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, (GLsizei)m_ucMSAALevel, GL_RGB8, m_vec2Size.x, m_vec2Size.y, GL_TRUE );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_glMSAA, 0 );
		glRenderbufferStorageMultisample( GL_RENDERBUFFER, (GLsizei)m_ucMSAALevel, GL_DEPTH24_STENCIL8, m_vec2Size.x, m_vec2Size.y );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_glMSAARBO );
	}
	else
	{
		m_glMSAAFBO = 0;
	}
}

void CFramebufferDefault::DestroyMSAABuffers( void )
{
	if (m_glMSAAFBO != 0)
	{
		glDeleteFramebuffers( 1, &m_glMSAAFBO );
		glDeleteTextures( 1, &m_glMSAA );
		glDeleteRenderbuffers( 1, &m_glMSAARBO );
	}
}