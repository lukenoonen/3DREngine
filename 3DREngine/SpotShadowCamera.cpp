#include "SpotShadowCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_DATADESC( CSpotShadowCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferShadow, m_fFramebufferShadow, "framebuffer", FL_NONE )

	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flNear, "near", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flFar, "far", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flOuterCutoff, "outercutoff", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flBlurRadius, "blurradius", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CSpotShadowCamera, camera_shadow_spot )


CSpotShadowCamera::CSpotShadowCamera()
{
	m_flNear = 0.1f;
	m_flFar = 1000.0f;
	m_flOuterCutoff = 0.7071f;
	m_flBlurRadius = 0.05f;
}

bool CSpotShadowCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	SetBlurScale( m_flBlurRadius.Get() / (tanf( m_flOuterCutoff.Get() ) * M_SQRT2) );
	
	return true;
}

void CSpotShadowCamera::Think( void )
{
	if (m_flOuterCutoff.Modified() || m_flBlurRadius.Modified())
		SetBlurScale( m_flBlurRadius.Get() / (tanf( m_flOuterCutoff.Get() ) * M_SQRT2) );

	BaseClass::Think();
}

void CSpotShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal[0] );

	BaseClass::ActivateLight();
}

CBaseFramebuffer *CSpotShadowCamera::GetFramebuffer( void )
{
	return &m_fFramebufferShadow;
}

const CBaseFramebuffer *CSpotShadowCamera::GetFramebuffer( void ) const
{
	return &m_fFramebufferShadow;
}

void CSpotShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
}

void CSpotShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
}

void CSpotShadowCamera::SetOuterCutoff( float flOuterCutoff )
{
	m_flOuterCutoff = flOuterCutoff;
}

void CSpotShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
}

const glm::mat4 &CSpotShadowCamera::GetView( void ) const
{
	return m_matView;
}

const glm::mat4 &CSpotShadowCamera::GetProjection( void ) const
{
	return m_matProjection;
}

const glm::mat4 &CSpotShadowCamera::GetTotal( void ) const
{
	return m_matTotal;
}

void CSpotShadowCamera::PerformRender( void )
{
	pRenderManager->SetRenderPass( ERenderPass::t_shadowspot );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal);

	pEntityManager->DrawUnlitEntities();
}

bool CSpotShadowCamera::ShouldUpdateView( void ) const
{
	return PositionUpdated() || RotationUpdated();
}

void CSpotShadowCamera::UpdateView( void )
{
	m_matView = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vec3Front, g_vec3Up );
}

bool CSpotShadowCamera::ShouldUpdateProjection( void ) const
{
	return m_flOuterCutoff.Modified() || m_flNear.Modified() || m_flFar.Modified();
}

void CSpotShadowCamera::UpdateProjection( void )
{
	m_matProjection = glm::perspective( m_flOuterCutoff.Get() * 2.0f, 1.0f, m_flNear.Get(), m_flFar.Get() );
}

void CSpotShadowCamera::UpdateTotal( void )
{
	m_matTotal = m_matProjection * m_matView;
}