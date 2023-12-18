#include "DirShadowCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "FramebufferShadow.h"

DEFINE_DATADESC( CDirShadowCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferShadow, m_pFramebuffer, "framebuffer", 0 )

	DEFINE_FIELD( DataField, float, m_flWidth, "width", 0 )
	DEFINE_FIELD( DataField, float, m_flNear, "near", 0 )
	DEFINE_FIELD( DataField, float, m_flFar, "far", 0 )
	DEFINE_FIELD( DataField, float, m_flBlurRadius, "blurradius", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CDirShadowCamera, camera_shadow_dir )

CDirShadowCamera::CDirShadowCamera()
{
	m_pFramebuffer = new CFramebufferShadow();

	m_flWidth = 16.0f;
	m_flNear = -16.0f;
	m_flFar = 16.0f;
	m_flBlurRadius = 0.05f;

	m_matView.resize( 1 );
	m_matProjection.resize( 1 );
	m_matTotal.resize( 1 );
}

bool CDirShadowCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateBlurScale = false;

	SetBlurScale( m_flBlurRadius / (m_flWidth * 2.0f) ); // TODO: test if this should be m_flWidth, or what it should be to be kept consistent

	return true;
}

void CDirShadowCamera::PostThink( void )
{
	if (m_bUpdateBlurScale)
	{
		SetBlurScale( m_flBlurRadius / (m_flWidth * 2.0f) );
	}

	BaseClass::PostThink();

	m_bUpdateBlurScale = false;
}

void CDirShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal[0]);

	BaseClass::ActivateLight();
}

void CDirShadowCamera::SetWidth( float flWidth )
{
	m_flWidth = flWidth * 0.5f;
	MarkUpdateProjection();
	m_bUpdateBlurScale = true;
}

void CDirShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	MarkUpdateProjection();
}

void CDirShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	MarkUpdateProjection();
}

void CDirShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
	m_bUpdateBlurScale = true;
}

void CDirShadowCamera::PerformRender( void )
{
	pRenderManager->SetRenderPass( ERenderPass::t_shadowdir );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal[0] );

	pEntityManager->DrawUnlitEntities();
}

void CDirShadowCamera::UpdateView( void )
{
	m_matView[0] = glm::lookAt(GetPosition(), GetPosition() + GetRotation() * g_vec3Front, GetRotation() * g_vec3Up);
}

void CDirShadowCamera::UpdateProjection( void )
{
	const glm::ivec2 &vec2Size = m_pFramebuffer->GetSize();
	float flHeight = m_flWidth * vec2Size.y / vec2Size.x;
	m_matProjection[0] = glm::ortho( -m_flWidth, m_flWidth, -flHeight, flHeight, m_flNear, m_flFar );
}

void CDirShadowCamera::UpdateTotal( void )
{
	m_matTotal[0] = m_matProjection[0] * m_matView[0];
}