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

int CFramebufferDefault::Bind( void ) const
{
	return pRenderManager->BindTexture( m_glTexture, GL_TEXTURE_2D );
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
	return m_glMSAAFBO != 0 ? m_glMSAAFBO : m_glTextureFBO;
}

void CFramebufferDefault::Blit( void )
{
	if (m_glMSAAFBO != 0)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, m_glMSAAFBO );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_glTextureFBO );
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

void CFramebufferDefault::ClearBufferInternal( void )
{
	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
}

void CFramebufferDefault::AdditionalClearBufferInternal( void )
{
	glClear( GL_DEPTH_BUFFER_BIT );
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

#include <iostream>

void CFramebufferDefault::CreateTextureBuffers( void )
{
	glGenFramebuffers( 1, &m_glTextureFBO );
	glGenRenderbuffers( 1, &m_glTextureRBO );
	glGenTextures( 1, &m_glTexture );

	glBindTexture( GL_TEXTURE_2D, m_glTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, m_vec2Size.x, m_vec2Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT );

	glBindFramebuffer( GL_FRAMEBUFFER, m_glTextureFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_glTexture, 0 );

	glBindRenderbuffer( GL_RENDERBUFFER, m_glTextureRBO );
	glRenderbufferStorage( GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_vec2Size.x, m_vec2Size.y );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_glTextureRBO );

	if (glCheckFramebufferStatus( GL_FRAMEBUFFER ) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete" << ": " << std::hex << glCheckFramebufferStatus( GL_FRAMEBUFFER ) << std::endl;
}

void CFramebufferDefault::DestroyTextureBuffers( void )
{
	pRenderManager->UnbindTexture( m_glTexture );

	glDeleteFramebuffers( 1, &m_glTextureFBO );
	glDeleteRenderbuffers( 1, &m_glTextureRBO );
	glDeleteTextures( 1, &m_glTexture );
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