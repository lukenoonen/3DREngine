#include "SpotShadowCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_DATADESC( CSpotShadowCamera )

	DEFINE_FIELD( DataField, float, m_flNear, "near", 0 )
	DEFINE_FIELD( DataField, float, m_flFar, "far", 0 )
	DEFINE_FIELD( DataField, float, m_flOuterCutoff, "outercutoff", 0 )
	DEFINE_FIELD( DataField, float, m_flBlurRadius, "blurradius", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CSpotShadowCamera, camera_shadow_spot )


CSpotShadowCamera::CSpotShadowCamera()
{
	m_flNear = 0.1f;
	m_flFar = 1000.0f;
	m_flOuterCutoff = 0.7071f;
	m_flBlurRadius = 0.05f;

	SetSizeRatio( 1.0f );
}

bool CSpotShadowCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateProjection = false;
	m_bUpdateBlurScale = false;

	m_matProjection = glm::perspective( m_flOuterCutoff * 2.0f, 1.0f, m_flNear, m_flFar );
	m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vec3Front, GetRotation() * g_vec3Up );
	m_matTotal = m_matProjection * m_matView;

	SetBlurScale( m_flBlurRadius / (tanf( m_flOuterCutoff ) * M_SQRT2) );
	
	return true;
}

void CSpotShadowCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (m_bUpdateProjection)
	{
		m_matProjection = glm::perspective( m_flOuterCutoff * 2.0f, 1.0f, m_flNear, m_flFar );
		m_bUpdateBlurScale = false;

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
	}

	if (m_bUpdateBlurScale)
	{
		SetBlurScale( m_flBlurRadius / (tanf( m_flOuterCutoff ) * M_SQRT2) );
		m_bUpdateBlurScale = false;
	}

	BaseClass::PostThink();
}

void CSpotShadowCamera::Render( void )
{
	pRenderManager->SetViewportSize( GetSize() );
	pRenderManager->SetFrameBuffer( m_glTextureFBO );

	pRenderManager->SetRenderPass( ERenderPass::t_shadowspot );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal );

	pEntityManager->DrawUnlitEntities();
}

void CSpotShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal );

	BaseClass::ActivateLight();
}

int CSpotShadowCamera::BindTexture( void )
{
	return pRenderManager->BindTexture( m_glTexture, GL_TEXTURE_2D );
}

void CSpotShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	m_bUpdateProjection = true;
}

void CSpotShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	m_bUpdateProjection = true;
}

void CSpotShadowCamera::SetOuterCutoff( float flOuterCutoff )
{
	m_flOuterCutoff = flOuterCutoff;
	m_bUpdateProjection = true;
	m_bUpdateBlurScale = true;
}

void CSpotShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
	m_bUpdateBlurScale = true;
}

void CSpotShadowCamera::CreateTextureBuffers( void )
{
	const glm::ivec2 &vec2Size = GetSize();

	glGenFramebuffers( 1, &m_glTextureFBO );
	glGenTextures( 1, &m_glTexture );

	glBindTexture( GL_TEXTURE_2D, m_glTexture );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, vec2Size.x, vec2Size.y, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
	glBindFramebuffer( GL_FRAMEBUFFER, m_glTextureFBO );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_glTexture, 0 );
	glDrawBuffer( GL_NONE );
	glReadBuffer( GL_NONE );
}

void CSpotShadowCamera::DestroyTextureBuffers( void )
{
	pRenderManager->UnbindTexture( m_glTexture );

	glDeleteFramebuffers( 1, &m_glTextureFBO );
	glDeleteTextures( 1, &m_glTexture );
}