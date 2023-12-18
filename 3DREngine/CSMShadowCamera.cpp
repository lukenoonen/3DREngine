#include "CSMShadowCamera.h"
#include "BasePlayerCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "BasePlayer.h"
#include "FramebufferShadowCSM.h"

DEFINE_DATADESC( CCSMShadowCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferShadowCSM, m_pFramebuffer, "framebuffer", 0 )

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
	m_pFramebuffer = new CFramebufferShadowCSM();

	m_flBlendDistance = 6.0f;
	m_flDistanceFactor = 4.0f;
	m_flInitialDistance = 6.0f;
	m_flNearError = -60.0f;
	m_flFarError = 60.0f;
	m_flBlurRadius = 0.05f;

	m_matView.resize( 1 );
	m_matProjection.resize( 4 );
	m_matTotal.resize( 4 );
}

bool CCSMShadowCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateCascade = false;
	m_bUpdateRadius = false;
	m_bUpdateNearFar = false;
	m_bUpdateBlurScale = false;

	CalculateCascade();
	CalculateRadius();
	CalculateNearFar();

	SetBlurScale( m_flBlurRadius * 0.5f / m_flRadius[0] );

	return true;
}

void CCSMShadowCamera::PostThink( void )
{
	m_bUpdateCascade = m_bUpdateCascade || cf_r_near.WasDispatched();
	m_bUpdateRadius = m_bUpdateCascade || cf_r_fov.WasDispatched() || cv_r_windowsize.WasDispatched();
	m_bUpdateNearFar = m_bUpdateNearFar || m_bUpdateCascade || cf_r_far.WasDispatched();
	m_bUpdateBlurScale = m_bUpdateBlurScale || m_bUpdateRadius;

	if (m_bUpdateCascade)
	{
		CalculateCascade();
	}

	if (m_bUpdateRadius)
	{
		CalculateRadius();
	}

	if (m_bUpdateNearFar)
	{
		CalculateNearFar();
	}

	if (m_bUpdateBlurScale)
	{
		SetBlurScale( m_flBlurRadius * 0.5f / m_flRadius[0] );
	}

	BaseClass::PostThink();

	m_bUpdateCascade = false;
	m_bUpdateRadius = false;
	m_bUpdateNearFar = false;
	m_bUpdateBlurScale = false;
}

void CCSMShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 4, &m_matTotal[0]);
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadowcascadefade, 0, &m_vec4CascadeEndClipSpaceNear );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadowcascadefade, 1, &m_vec4CascadeEndClipSpaceFar );

	BaseClass::ActivateLight();
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
	MarkUpdateProjection();
}

void CCSMShadowCamera::SetFarError( float flFarError )
{
	m_flFarError = flFarError;
	MarkUpdateProjection();
}

void CCSMShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
	m_bUpdateBlurScale = true;
}

void CCSMShadowCamera::PerformRender( void )
{
	float flShadowSize = (float)m_pFramebuffer->GetSize().x;

	pRenderManager->SetRenderPass( ERenderPass::t_shadowcsm );

	for (unsigned char i = 0; i < 4; i++)
	{
		pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal[i] );

		pRenderManager->SetViewportOffset( glm::ivec2( flShadowSize * (float)i, 0 ) );

		pEntityManager->DrawUnlitEntities();
	}

	pRenderManager->SetViewportOffset( g_vec2Zero );
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

void CCSMShadowCamera::UpdateView( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetPlayer( 0 )->GetCamera();
	m_matView[0] = glm::lookAt( pPlayerCamera->GetPosition() - GetRotation() * g_vec3Front, pPlayerCamera->GetPosition(), GetRotation() * g_vec3Up );
}

void CCSMShadowCamera::UpdateProjection( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetPlayer( 0 )->GetCamera();

	float flShadowSize = (float)m_pFramebuffer->GetSize().x;

	for (unsigned char i = 0; i < 4; i++)
	{
		glm::vec3 vec3FrustumCenter = pPlayerCamera->GetPosition() + pPlayerCamera->GetRotation() * g_vec3Front * ((m_flCascadeEnd[i + 1] + m_flCascadeEnd[i]) * 0.5f);
		glm::vec3 maxOrtho = glm::vec3( m_matView[0] * glm::vec4( vec3FrustumCenter, 1.0f ) ) + glm::vec3( m_flRadius[i] );
		glm::vec3 minOrtho = glm::vec3( m_matView[0] * glm::vec4( vec3FrustumCenter, 1.0f ) ) - glm::vec3( m_flRadius[i] );

		m_matProjection[i] = glm::ortho( minOrtho.x, maxOrtho.x, minOrtho.y, maxOrtho.y, minOrtho.z + m_flNearError, maxOrtho.z + m_flFarError );
		glm::vec4 vec4ShadowOrigin = ((m_matProjection[i] * m_matView[0]) * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f )) * flShadowSize * 0.5f;
		glm::vec4 vec4RoundOffset = glm::round( vec4ShadowOrigin ) - vec4ShadowOrigin;
		vec4RoundOffset = vec4RoundOffset * 2.0f / flShadowSize;
		vec4RoundOffset.z = 0.0f;
		vec4RoundOffset.w = 0.0f;
		m_matProjection[i] += vec4RoundOffset;
	}
}

void CCSMShadowCamera::UpdateTotal( void )
{
	for (unsigned char i = 0; i < 4; i++)
	{
		m_matTotal[i] = m_matProjection[i] * m_matView[0];
	}
}

bool CCSMShadowCamera::ShouldUpdateView( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetPlayer( 0 )->GetCamera();
	return pPlayerCamera->PositionUpdated() || RotationUpdated();
}

bool CCSMShadowCamera::ShouldUpdateProjection( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetPlayer( 0 )->GetCamera();
	return BaseClass::ShouldUpdateProjection() || m_bUpdateRadius || pPlayerCamera->PositionUpdated() || pPlayerCamera->RotationUpdated() || RotationUpdated() || cf_r_vcsizefactor.WasDispatched();
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