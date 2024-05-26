#include "CSMShadowCamera.h"
#include "BasePlayerCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "BasePlayer.h"

DEFINE_DATADESC( CCSMShadowCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferShadowCSM, m_fFramebufferShadowCSM, "framebuffer", FL_NONE )

	DEFINE_FIELD( DataField, float, m_flBlendDistance, "blenddistance", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flDistanceFactor, "distancefactor", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flInitialDistance, "initialdistance", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flNearError, "nearerror", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flFarError, "farerror", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flBlurRadius, "blurradius", FL_NONE )

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

	m_bUpdateProjection = false;
}

bool CCSMShadowCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	CalculateCascade();
	CalculateRadius();
	CalculateNearFar();
	CalculateBlurScale();

	return true;
}

void CCSMShadowCamera::Think( void )
{
	bool bUpdateCascade = m_flBlendDistance.Modified() || m_flDistanceFactor.Modified() || m_flInitialDistance.Modified() || cf_r_near.WasDispatched();
	bool bUpdateRadius = bUpdateCascade || cf_r_fov.WasDispatched() || cv_r_windowsize.WasDispatched();
	bool bUpdateNearFar = bUpdateCascade || cf_r_far.WasDispatched();
	bool bUpdateBlurScale = bUpdateRadius || m_flBlurRadius.Modified();

	if (bUpdateCascade) CalculateCascade();
	if (bUpdateRadius) CalculateRadius();
	if (bUpdateNearFar) CalculateNearFar();
	if (bUpdateBlurScale) CalculateBlurScale();

	m_bUpdateProjection = m_flNearError.Modified() || m_flFarError.Modified() || bUpdateCascade || bUpdateRadius;

	BaseClass::Think();
}

void CCSMShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 4, m_matTotal);
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadowcascadefade, 0, &m_vec4CascadeEndClipSpaceNear );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadowcascadefade, 1, &m_vec4CascadeEndClipSpaceFar );

	BaseClass::ActivateLight();
}

CBaseFramebuffer *CCSMShadowCamera::GetFramebuffer( void )
{
	return &m_fFramebufferShadowCSM;
}

const CBaseFramebuffer *CCSMShadowCamera::GetFramebuffer( void ) const
{
	return &m_fFramebufferShadowCSM;
}

void CCSMShadowCamera::SetBlendDistance( float flBlendDistance )
{
	m_flBlendDistance = flBlendDistance;
}

void CCSMShadowCamera::SetDistanceFactor( float flDistanceFactor )
{
	m_flDistanceFactor = flDistanceFactor;
}

void CCSMShadowCamera::SetInitialDistance( float flInitialDistance )
{
	m_flInitialDistance = flInitialDistance;
}

void CCSMShadowCamera::SetNearError( float flNearError )
{
	m_flNearError = flNearError;
}

void CCSMShadowCamera::SetFarError( float flFarError )
{
	m_flFarError = flFarError;
}

void CCSMShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
}

const glm::mat4 &CCSMShadowCamera::GetView( void ) const
{
	return m_matView;
}

const glm::mat4 &CCSMShadowCamera::GetProjection( void ) const
{
	return m_matProjection[0];
}

const glm::mat4 &CCSMShadowCamera::GetTotal( void ) const
{
	return m_matTotal[0];
}

void CCSMShadowCamera::PerformRender( void )
{
	float flShadowSize = (float)m_fFramebufferShadowCSM.GetSize().x;

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
	float flBlendDistance = m_flBlendDistance.Get();

	m_flCascadeEnd[0] = cf_r_near.GetValue();
	float flDistance = m_flInitialDistance.Get() + m_flCascadeEnd[0];
	for (unsigned char i = 0; i < 4; i++)
	{
		m_flCascadeEnd[i + 1] = flDistance;
		m_flCascadeEndNear[i] = flDistance - flBlendDistance;

		flBlendDistance = (m_flDistanceFactor.Get() - 1.0f) * (flBlendDistance + m_flCascadeEnd[i]);
		flDistance *= m_flDistanceFactor.Get();
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

bool CCSMShadowCamera::ShouldUpdateView( void ) const
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetLocalPlayer()->GetCamera();
	return pPlayerCamera->PositionUpdated() || RotationUpdated();
}

void CCSMShadowCamera::UpdateView( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetLocalPlayer()->GetCamera();
	m_matView = glm::lookAt( pPlayerCamera->GetPosition() - GetRotation() * g_vec3Front, pPlayerCamera->GetPosition(), g_vec3Up );
}

bool CCSMShadowCamera::ShouldUpdateProjection( void ) const
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetLocalPlayer()->GetCamera();
	return m_bUpdateProjection || pPlayerCamera->PositionUpdated() || pPlayerCamera->RotationUpdated() || RotationUpdated();
}

void CCSMShadowCamera::UpdateProjection( void )
{
	CBasePlayerCamera *pPlayerCamera = pEntityManager->GetLocalPlayer()->GetCamera();

	float flShadowSize = (float)m_fFramebufferShadowCSM.GetSize().x;

	for (unsigned char i = 0; i < 4; i++)
	{
		glm::vec3 vec3FrustumCenter = pPlayerCamera->GetPosition() + pPlayerCamera->GetRotation() * g_vec3Front * ((m_flCascadeEnd[i + 1] + m_flCascadeEnd[i]) * 0.5f);
		glm::vec3 maxOrtho = glm::vec3( m_matView * glm::vec4( vec3FrustumCenter, 1.0f ) ) + glm::vec3( m_flRadius[i] );
		glm::vec3 minOrtho = glm::vec3( m_matView * glm::vec4( vec3FrustumCenter, 1.0f ) ) - glm::vec3( m_flRadius[i] );

		m_matProjection[i] = glm::ortho( minOrtho.x, maxOrtho.x, minOrtho.y, maxOrtho.y, minOrtho.z + m_flNearError.Get(), maxOrtho.z + m_flFarError.Get() );
		glm::vec4 vec4ShadowOrigin = ((m_matProjection[i] * m_matView) * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f )) * flShadowSize * 0.5f;
		glm::vec4 vec4RoundOffset = glm::round( vec4ShadowOrigin ) - vec4ShadowOrigin;
		vec4RoundOffset = vec4RoundOffset * 2.0f / flShadowSize;
		vec4RoundOffset.z = 0.0f;
		vec4RoundOffset.w = 0.0f;
		m_matProjection[i][3] += vec4RoundOffset;
	}

	m_bUpdateProjection = false;
}

void CCSMShadowCamera::UpdateTotal( void )
{
	for (unsigned char i = 0; i < 4; i++)
		m_matTotal[i] = m_matProjection[i] * m_matView;
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

void CCSMShadowCamera::CalculateBlurScale( void )
{
	SetBlurScale( m_flBlurRadius.Get() * 0.5f / m_flRadius[0] );
}