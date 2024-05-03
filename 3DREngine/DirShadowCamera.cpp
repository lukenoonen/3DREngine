#include "DirShadowCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "FramebufferShadow.h"

DEFINE_DATADESC( CDirShadowCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferShadow, m_pFramebuffer, "framebuffer", FL_NONE )

	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flWidth, "width", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flNear, "near", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flFar, "far", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flBlurRadius, "blurradius", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CDirShadowCamera, camera_shadow_dir )

CDirShadowCamera::CDirShadowCamera()
{
	InitFramebuffer( new CFramebufferShadow() );

	m_flWidth = 16.0f;
	m_flNear = -16.0f;
	m_flFar = 16.0f;
	m_flBlurRadius = 0.05f;
}

bool CDirShadowCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	SetBlurScale( m_flBlurRadius.Get() / (m_flWidth.Get() * 2.0f) ); // TODO: test if this should be m_flWidth, or what it should be to be kept consistent

	return true;
}

void CDirShadowCamera::Think( void )
{
	if (m_flBlurRadius.Modified() || m_flWidth.Modified())
		SetBlurScale( m_flBlurRadius.Get() / (m_flWidth.Get() * 2.0f) );

	BaseClass::Think();
}

void CDirShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal);

	BaseClass::ActivateLight();
}

void CDirShadowCamera::SetWidth( float flWidth )
{
	m_flWidth = flWidth * 0.5f;
}

void CDirShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
}

void CDirShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
}

void CDirShadowCamera::SetBlurRadius( float flBlurRadius )
{
	m_flBlurRadius = flBlurRadius;
}

const glm::mat4 &CDirShadowCamera::GetView( void ) const
{
	return m_matView;
}

const glm::mat4 &CDirShadowCamera::GetProjection( void ) const
{
	return m_matProjection;
}

const glm::mat4 &CDirShadowCamera::GetTotal( void ) const
{
	return m_matTotal;
}

void CDirShadowCamera::PerformRender( void )
{
	pRenderManager->SetRenderPass( ERenderPass::t_shadowdir );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 1, &m_matTotal );

	pEntityManager->DrawUnlitEntities();
}

bool CDirShadowCamera::ShouldUpdateView( void ) const
{
	return PositionUpdated() || RotationUpdated();
}

void CDirShadowCamera::UpdateView( void )
{
	m_matView = glm::lookAt(GetPosition(), GetPosition() + GetRotation() * g_vec3Front, g_vec3Up);
}

bool CDirShadowCamera::ShouldUpdateProjection( void ) const
{
	return m_flWidth.Modified() || m_flNear.Modified() || m_flFar.Modified();
}

void CDirShadowCamera::UpdateProjection( void )
{
	const glm::ivec2 &vec2Size = m_pFramebuffer->GetSize();
	float flHeight = m_flWidth.Get() * vec2Size.y / vec2Size.x;
	m_matProjection = glm::ortho( -m_flWidth.Get(), m_flWidth.Get(), -flHeight, flHeight, m_flNear.Get(), m_flFar.Get() );
}

void CDirShadowCamera::UpdateTotal( void )
{
	m_matTotal = m_matProjection * m_matView;
}