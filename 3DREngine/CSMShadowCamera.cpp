#include "CSMShadowCamera.h"
#include "BasePlayerCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_DATADESC( CCSMShadowCamera )

	DEFINE_FIELD( DataField, float, m_flBlendDistance, "blenddistance", 0 )
	DEFINE_FIELD( DataField, float, m_flDistanceFactor, "distancefactor", 0 )
	DEFINE_FIELD( DataField, float, m_flInitialDistance, "initialdistance", 0 )
	DEFINE_FIELD( DataField, float, m_flNearError, "nearerror", 0 )
	DEFINE_FIELD( DataField, float, m_flFarError, "farerror", 0 )
	DEFINE_FIELD( DataField, float, m_flBlurRadius, "blurradius", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CCSMShadowCamera, camera_shadow_csm )

CCSMShadowCamera::CCSMShadowCamera()
{
	m_flBlendDistance = 6.0f;
	m_flDistanceFactor = 4.0f;
	m_flInitialDistance = 6.0f;
	m_flNearError = -60.0f;
	m_flFarError = 60.0f;
	m_flBlurRadius = 0.05f;

	SetSizeRatio( 4.0f );
}

bool CCSMShadowCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateCascade = false;
	m_bUpdateRadius = false;
	m_bUpdateTotal = false;
	m_bUpdateNearFar = false;
	m_bUpdateBlurScale = false;

	CalculateCascade();
	CalculateRadius();
	CalculateTotal();
	CalculateNearFar();

	SetBlurScale( m_flBlurRadius * 0.5f / m_flRadius[0] );

	return true;
}

void CCSMShadowCamera::PostThink( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetPlayer( 0 )->GetCamera();
	m_bUpdateCascade = m_bUpdateCascade || cf_r_near.WasDispatched();
	m_bUpdateRadius = m_bUpdateCascade || cf_r_fov.WasDispatched() || cv_r_windowsize.WasDispatched();
	m_bUpdateTotal = m_bUpdateTotal || m_bUpdateRadius || pPlayerCamera->PositionUpdated() || pPlayerCamera->RotationUpdated() || RotationUpdated() || cf_r_vcsizefactor.WasDispatched(); // TODO: swap cf_r_vcsizefactor for on size change
	m_bUpdateNearFar = m_bUpdateNearFar || m_bUpdateCascade || cf_r_far.WasDispatched();
	m_bUpdateBlurScale = m_bUpdateBlurScale || m_bUpdateRadius;

	if (m_bUpdateCascade)
	{
		CalculateCascade();
		m_bUpdateCascade = false;
	}

	if (m_bUpdateRadius)
	{
		CalculateRadius();
		m_bUpdateRadius = false;
	}

	if (m_bUpdateTotal)
	{
		CalculateTotal();
		m_bUpdateTotal = false;
	}

	if (m_bUpdateNearFar)
	{
		CalculateNearFar();
		m_bUpdateNearFar = false;
	}

	if (m_bUpdateBlurScale)
	{
		SetBlurScale( m_flBlurRadius * 0.5f / m_flRadius[0] );
		m_bUpdateBlurScale = false;
	}

	BaseClass::PostThink();
}

void CCSMShadowCamera::Render( void )
{
	glm::vec2 vec2Size = glm::ivec2( GetSize().y );

	pRenderManager->SetViewportSize( vec2Size );
	pRenderManager->SetFrameBuffer( m_glTextureFBO );

	pRenderManager->SetRenderPass( ERenderPass::t_shadowcsm );

	for (unsigned char i = 0; i < 4; i++)
	{
		pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal[i] );

		pRenderManager->SetViewportOffset( glm::ivec2( vec2Size.x * (float)i, 0 ) );

		pEntityManager->DrawUnlitEntities();
	}

	pRenderManager->SetViewportOffset( g_vec2Zero );
}

void CCSMShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 4, m_matTotal );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadowcascadefade, 0, &m_vec4CascadeEndClipSpaceNear );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadowcascadefade, 1, &m_vec4CascadeEndClipSpaceFar );

	BaseClass::ActivateLight();
}

int CCSMShadowCamera::BindTexture( void )
{
	return pRenderManager->BindTexture( m_glTexture, GL_TEXTURE_2D );
}

void CCSMShadowCamera::SetBlendDistance( float flBlendDistance )
{
	m_flBlendDistance = flBlendDistance;
	m_bUpdateCascade = true;
}

void CCSMShadowCamera::SetDistanceFactor( float flDistanceFactor )
{
	m_flDistanceFactor = flDistanceFactor;
	m_bUpdateCascade = true;
}

void CCSMShadowCamera::SetInitialDistance( float flInitialDistance )
{
	m_flInitialDistance = flInitialDistance;
	m_bUpdateCascade = true;
}

void CCSMShadowCamera::SetNearError( float flNearError )
{
	m_flNearError = flNearError;
	m_bUpdateTotal = true;
}

void CCSMShadowCamera::SetFarError( float flFarError )
{
	m_flFarError = flFarError;
	m_bUpdateTotal = true;
}

void CCSMShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
	m_bUpdateBlurScale = true;
}

void CCSMShadowCamera::CreateTextureBuffers( void )
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

void CCSMShadowCamera::DestroyTextureBuffers( void )
{
	pRenderManager->UnbindTexture( m_glTexture );

	glDeleteFramebuffers( 1, &m_glTextureFBO );
	glDeleteTextures( 1, &m_glTexture );
}

void CCSMShadowCamera::CalculateCascade( void )
{
	float flBlendDistance = m_flBlendDistance;

	m_flCascadeEnd[0] = cf_r_near.GetValue();
	float flDistance = m_flInitialDistance + m_flCascadeEnd[0];
	for (unsigned char i = 0; i < 4; i++)
	{
		m_flCascadeEnd[i + 1] = flDistance;
		m_flCascadeEndNear[i] = flDistance - flBlendDistance;

		flBlendDistance = (m_flDistanceFactor - 1.0f) * (flBlendDistance + m_flCascadeEnd[i]);
		flDistance *= m_flDistanceFactor;
	}
}

void CCSMShadowCamera::CalculateRadius( void )
{
	for (unsigned char i = 0; i < 4; i++)
	{
		float farHeight = tanf( glm::radians( cf_r_fov.GetValue() ) * 0.5f ) * m_flCascadeEnd[i + 1];
		float farWidth = farHeight * cv_r_windowsize.GetValue().x / cv_r_windowsize.GetValue().y;
		m_flRadius[i] = glm::sqrt( m_flCascadeEnd[i + 1] * m_flCascadeEnd[i + 1] + farHeight * farHeight + farWidth * farWidth );
	}
}

void CCSMShadowCamera::CalculateTotal( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetPlayer( 0 )->GetCamera();

	float flShadowSize = (float)GetSize().y; // TODO: check if this is needed, and if the Init function should be reverted

	glm::mat4 matLightView = glm::lookAt( pPlayerCamera->GetPosition() - GetRotation() * g_vec3Front, pPlayerCamera->GetPosition(), GetRotation() * g_vec3Up );

	for (unsigned char i = 0; i < 4; i++)
	{
		glm::vec3 vec3FrustumCenter = pPlayerCamera->GetPosition() + pPlayerCamera->GetRotation() * g_vec3Front * ((m_flCascadeEnd[i + 1] + m_flCascadeEnd[i]) * 0.5f);
		glm::vec3 maxOrtho = glm::vec3( matLightView * glm::vec4( vec3FrustumCenter, 1.0f ) ) + glm::vec3( m_flRadius[i] );
		glm::vec3 minOrtho = glm::vec3( matLightView * glm::vec4( vec3FrustumCenter, 1.0f ) ) - glm::vec3( m_flRadius[i] );

		glm::mat4 matLightOrtho = glm::ortho( minOrtho.x, maxOrtho.x, minOrtho.y, maxOrtho.y, minOrtho.z + m_flNearError, maxOrtho.z + m_flFarError );
		glm::vec4 vec4ShadowOrigin = ((matLightOrtho * matLightView) * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f )) * flShadowSize * 0.5f;
		glm::vec4 vec4RoundOffset = glm::round( vec4ShadowOrigin ) - vec4ShadowOrigin;
		vec4RoundOffset = vec4RoundOffset * 2.0f / flShadowSize;
		vec4RoundOffset.z = 0.0f;
		vec4RoundOffset.w = 0.0f;
		matLightOrtho[3] += vec4RoundOffset;

		m_matTotal[i] = matLightOrtho * matLightView;
	}
}

void CCSMShadowCamera::CalculateNearFar( void )
{
	for (unsigned char i = 0; i < 4; i++)
	{
		float flNear = cf_r_near.GetValue();
		float flFar = cf_r_far.GetValue();

		float flFarPlusNear = flFar + flNear;
		float flTwoTimesFarTimesNear = 2.0f * flFar * flNear;
		float flInverseFarMinusNear = 1.0f / (flFar - flNear);

		m_vec4CascadeEndClipSpaceNear[i] = (m_flCascadeEndNear[i] * flFarPlusNear - flTwoTimesFarTimesNear) * flInverseFarMinusNear;
		m_vec4CascadeEndClipSpaceFar[i] = (m_flCascadeEnd[i + 1] * flFarPlusNear - flTwoTimesFarTimesNear) * flInverseFarMinusNear;
	}
}