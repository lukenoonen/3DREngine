#include "PointShadowCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

CPointShadowCamera::CPointShadowCamera()
{
	m_flNear = 0.1f;
	m_flFar = 1000.0f;

	SetSizeRatio( 1.0f );
}

bool CPointShadowCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateProjection = false;

	m_matProjection = glm::perspective( M_PI * 0.5f, 1.0f, m_flNear, m_flFar );
	m_matView[0] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Left, g_vec3Up );
	m_matView[1] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Right, g_vec3Up );
	m_matView[2] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Down, g_vec3Back );
	m_matView[3] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Up, g_vec3Front );
	m_matView[4] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Back, g_vec3Up );
	m_matView[5] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Front, g_vec3Up );
	for (unsigned char i = 0; i < 6; i++)
		m_matTotal[i] = m_matProjection * m_matView[i];
	
	return true;
}

void CPointShadowCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (m_bUpdateProjection)
	{
		m_matProjection = glm::perspective( M_PI * 0.5f, 1.0f, m_flNear, m_flFar );
		m_bUpdateProjection = false;

		bUpdateTotal = true;
	}

	if (PositionUpdated())
	{
		m_matView[0] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Left, g_vec3Up );
		m_matView[1] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Right, g_vec3Up );
		m_matView[2] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Down, g_vec3Back );
		m_matView[3] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Up, g_vec3Front );
		m_matView[4] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Back, g_vec3Up );
		m_matView[5] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Front, g_vec3Up );

		bUpdateTotal = true;
	}

	if (bUpdateTotal)
	{
		for (unsigned char i = 0; i < 6; i++)
			m_matTotal[i] = m_matProjection * m_matView[i];
	}

	BaseClass::PostThink();
}

void CPointShadowCamera::Render( void )
{
	pRenderManager->SetViewportSize( GetSize() );
	pRenderManager->SetFrameBuffer( m_glTextureFBO );

	pRenderManager->SetRenderPass( ERenderPass::t_shadowpoint );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 6, m_matTotal );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightmaxdistance, 0, &m_flFar );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightposition, 0, &GetPosition() );

	pEntityManager->DrawUnlitEntities();
}

void CPointShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 6, m_matTotal );

	BaseClass::ActivateLight();
}

int CPointShadowCamera::BindTexture( void )
{
	return pRenderManager->BindTexture( m_glTexture, GL_TEXTURE_CUBE_MAP );
}

void CPointShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	m_bUpdateProjection = true;
}

void CPointShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	m_bUpdateProjection = true;
}

void CPointShadowCamera::SetBlurRadius( float flBlurRadius )
{
	SetBlurScale( flBlurRadius / M_SQRT2 );
}

void CPointShadowCamera::CreateTextureBuffers( void )
{
	const glm::ivec2 &vec2Size = GetSize();

	glGenFramebuffers( 1, &m_glTextureFBO );
	glGenTextures( 1, &m_glTexture );

	glBindTexture( GL_TEXTURE_CUBE_MAP, m_glTexture );
	for (unsigned char i = 0; i < 6; i++)
		glTexImage2D( GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT16, vec2Size.x, vec2Size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	glBindFramebuffer( GL_FRAMEBUFFER, m_glTextureFBO );
	glFramebufferTexture( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_glTexture, 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
}

void CPointShadowCamera::DestroyTextureBuffers( void )
{
	pRenderManager->UnbindTexture( m_glTexture );

	glDeleteFramebuffers( 1, &m_glTextureFBO );
	glDeleteTextures( 1, &m_glTexture );
}