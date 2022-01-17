#include "BaseCamera.h"
#include "RenderManager.h"

CBaseCamera::CBaseCamera( unsigned int uiRenderPriority )
{
	m_uiRenderPriority = uiRenderPriority;
}

bool CBaseCamera::IsCamera( void ) const
{
	return true;
}

void CBaseCamera::Render( void )
{
	/*pRenderManager->SetViewportSize(m_vecSize);
	pRenderManager->SetFrameBuffer( m_uiMSAALevel != 0 ? m_uiMSAAFBO : m_uiTextureFBO );

	// Draw stuff

	if (m_uiMSAALevel != 0)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, m_uiMSAAFBO );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, m_uiTextureFBO );
		glBlitFramebuffer( 0, 0, m_vecSize.x, m_vecSize.y, 0, 0, m_vecSize.x, m_vecSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}*/
}

void CBaseCamera::CreateTextureBuffers( void )
{

}

void CBaseCamera::DestroyTextureBuffers( void )
{

}

void CBaseCamera::CreateMSAABuffers( void )
{

}

void CBaseCamera::DestroyMSAABuffers( void )
{

}

unsigned int CBaseCamera::GetRenderPriority( void ) const
{
	return m_uiRenderPriority;
}

/*
void CBaseCamera::SetSize( const glm::ivec2 vecSize )
{
	if (m_vecSize != vecSize)
	{
		DestroyTextureBuffers();
		DestroyMSAABuffers();

		m_vecSize = vecSize;

		CreateMSAABuffers();
		CreateTextureBuffers();
	}
}

void CBaseCamera::SetMSAALevel( unsigned int uiMSAALevel )
{
	if (m_uiMSAALevel != uiMSAALevel)
	{
		DestroyMSAABuffers();

		m_uiMSAALevel = uiMSAALevel;

		CreateMSAABuffers();
	}
}

const glm::ivec2 &CBaseCamera::GetSize( void ) const
{
	return m_vecSize;
}

unsigned int CBaseCamera::GetMSAALevel( void ) const
{
	return m_uiMSAALevel;
}

void CBaseCamera::CreateTextureBuffers( void )
{
	if (m_bShadow)
	{
		glGenFramebuffers( 1, &m_uiTextureFBO );
		glGenTextures( 1, &m_uiTexture );

		glBindTexture( GL_TEXTURE_2D, m_uiTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, m_vecSize.x, m_vecSize.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
		glBindFramebuffer( GL_FRAMEBUFFER, m_uiTextureFBO );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_uiTexture, 0 );
		glDrawBuffer( GL_NONE );
		glReadBuffer( GL_NONE );
	}
	else if (m_bWorld)
	{
		glGenFramebuffers( 1, &m_uiTextureFBO );
		glGenTextures( 1, &m_uiTexture );

		glBindTexture( GL_TEXTURE_2D, m_uiTexture );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB8, m_vecSize.x, m_vecSize.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL );
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
	else
	{
		m_uiTextureFBO = 0;
		m_uiTexture = 0;
	}
}

void CBaseCamera::DestroyTextureBuffers( void )
{
	if (m_bShadow || m_bWorld)
	{
		glDeleteFramebuffers( 1, &m_uiTextureFBO );
		glDeleteTextures( 1, &m_uiTexture );
	}
}

void CBaseCamera::CreateMSAABuffers( void )
{
	if (m_uiMSAALevel != 0)
	{
		glGenFramebuffers( 1, &m_uiMSAAFBO );
		glGenTextures( 1, &m_uiMSAA );
		glGenRenderbuffers( 1, &m_uiMSAARBO );

		glBindFramebuffer( GL_FRAMEBUFFER, m_uiMSAAFBO );
		glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_uiMSAA );
		glBindRenderbuffer( GL_RENDERBUFFER, m_uiMSAARBO );

		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, m_uiMSAALevel, GL_RGB8, m_vecSize.x, m_vecSize.y, GL_TRUE );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_uiMSAA, 0 );
		glRenderbufferStorageMultisample( GL_RENDERBUFFER, m_uiMSAALevel, GL_DEPTH24_STENCIL8, m_vecSize.x, m_vecSize.y );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uiMSAARBO );
	}
}

void CBaseCamera::DestroyMSAABuffers( void )
{
	if (m_uiMSAALevel != 0)
	{
		glDeleteFramebuffers( 1, &m_uiMSAAFBO );
		glDeleteTextures( 1, &m_uiMSAA );
		glDeleteRenderbuffers( 1, &m_uiMSAARBO );
	}
}*/