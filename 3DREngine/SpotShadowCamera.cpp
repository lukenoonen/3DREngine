#include "SpotShadowCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "FramebufferShadow.h"

DEFINE_DATADESC( CSpotShadowCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferShadow, m_pFramebuffer, "framebuffer", 0 )

	DEFINE_FIELD( DataField, float, m_flNear, "near", 0 )
	DEFINE_FIELD( DataField, float, m_flFar, "far", 0 )
	DEFINE_FIELD( DataField, float, m_flOuterCutoff, "outercutoff", 0 )
	DEFINE_FIELD( DataField, float, m_flBlurRadius, "blurradius", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CSpotShadowCamera, camera_shadow_spot )


CSpotShadowCamera::CSpotShadowCamera()
{
	m_pFramebuffer = new CFramebufferShadow();

	m_flNear = 0.1f;
	m_flFar = 1000.0f;
	m_flOuterCutoff = 0.7071f;
	m_flBlurRadius = 0.05f;

	m_matView.resize( 1 );
	m_matProjection.resize( 1 );
	m_matTotal.resize( 1 );
}

bool CSpotShadowCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateBlurScale = false;

	SetBlurScale( m_flBlurRadius / (tanf( m_flOuterCutoff ) * M_SQRT2) );
	
	return true;
}

void CSpotShadowCamera::PostThink( void )
{
	if (m_bUpdateBlurScale)
	{
		SetBlurScale( m_flBlurRadius / (tanf( m_flOuterCutoff ) * M_SQRT2) );
	}

	BaseClass::PostThink();

	m_bUpdateBlurScale = false;
}

void CSpotShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal[0] );

	BaseClass::ActivateLight();
}

void CSpotShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	MarkUpdateProjection();
}

void CSpotShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	MarkUpdateProjection();
}

void CSpotShadowCamera::SetOuterCutoff( float flOuterCutoff )
{
	m_flOuterCutoff = flOuterCutoff;
	MarkUpdateProjection();
	m_bUpdateBlurScale = true;
}

void CSpotShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
	m_bUpdateBlurScale = true;
}

void CSpotShadowCamera::PerformRender( void )
{
	pRenderManager->SetRenderPass( ERenderPass::t_shadowspot );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal[0]);

	pEntityManager->DrawUnlitEntities();
}

void CSpotShadowCamera::UpdateView( void )
{
	m_matView[0] = glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vec3Front, GetRotation() * g_vec3Up );
}

void CSpotShadowCamera::UpdateProjection( void )
{
	m_matProjection[0] = glm::perspective( m_flOuterCutoff * 2.0f, 1.0f, m_flNear, m_flFar );
}

void CSpotShadowCamera::UpdateTotal( void )
{
	m_matTotal[0] = m_matProjection[0] * m_matView[0];
}