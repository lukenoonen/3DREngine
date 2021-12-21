#include "BaseMSAACamera.h"

CBaseMSAACamera::CBaseMSAACamera( unsigned int uiMSAALevel, const glm::vec2 &vecSize, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecSize, uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_uiMSAALevel = uiMSAALevel;

	CreateMSAABuffers();
}

void CBaseMSAACamera::SetMSAALevel( unsigned int uiMSAALevel )
{
	if (m_uiMSAALevel != uiMSAALevel)
	{
		DestroyMSAABuffers();

		m_uiMSAALevel = uiMSAALevel;

		CreateMSAABuffers();
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
		const glm::ivec2 &vecSize = GetSize();

		glGenFramebuffers( 1, &m_uiMSAAFBO );
		glGenTextures( 1, &m_uiMSAA );
		glGenRenderbuffers( 1, &m_uiMSAARBO );

		glBindFramebuffer( GL_FRAMEBUFFER, m_uiMSAAFBO );
		glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_uiMSAA );
		glBindRenderbuffer( GL_RENDERBUFFER, m_uiMSAARBO );

		glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, m_uiMSAALevel, GL_RGB8, vecSize.x, vecSize.y, GL_TRUE );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_uiMSAA, 0 );
		glRenderbufferStorageMultisample( GL_RENDERBUFFER, m_uiMSAALevel, GL_DEPTH24_STENCIL8, vecSize.x, vecSize.y );
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