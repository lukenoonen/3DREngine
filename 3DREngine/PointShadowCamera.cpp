#include "PointShadowCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "FramebufferShadowCubemap.h"

DEFINE_DATADESC( CPointShadowCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferShadowCubemap, m_pFramebuffer, "framebuffer", FL_NONE )

	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flNear, "near", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flFar, "far", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CPointShadowCamera, camera_shadow_point )

CPointShadowCamera::CPointShadowCamera()
{
	InitFramebuffer( new CFramebufferShadowCubemap() );

	m_flNear = 0.1f;
	m_flFar = 1000.0f;
}

void CPointShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 6, &m_matTotal );

	BaseClass::ActivateLight();
}

void CPointShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
}

void CPointShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
}

void CPointShadowCamera::SetBlurRadius( float flBlurRadius )
{
	SetBlurScale( flBlurRadius / M_SQRT2 );
}

const glm::mat4 &CPointShadowCamera::GetView( void ) const
{
	return m_matView[0];
}

const glm::mat4 &CPointShadowCamera::GetProjection( void ) const
{
	return m_matProjection;
}

const glm::mat4 &CPointShadowCamera::GetTotal( void ) const
{
	return m_matTotal[0];
}

void CPointShadowCamera::PerformRender( void )
{
	pRenderManager->SetRenderPass( ERenderPass::t_shadowpoint );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 6, &m_matTotal );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightmaxdistance, 0, &m_flFar );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightposition, 0, &GetPosition() );

	pEntityManager->DrawUnlitEntities();
}

bool CPointShadowCamera::ShouldUpdateView( void ) const
{
	return PositionUpdated();
}

void CPointShadowCamera::UpdateView( void )
{
	m_matView[0] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Left, g_vec3Up );
	m_matView[1] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Right, g_vec3Up );
	m_matView[2] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Down, g_vec3Back );
	m_matView[3] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Up, g_vec3Front );
	m_matView[4] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Back, g_vec3Up );
	m_matView[5] = glm::lookAt( GetPosition(), GetPosition() + g_vec3Front, g_vec3Up );
}

bool CPointShadowCamera::ShouldUpdateProjection( void ) const
{
	return m_flNear.Modified() || m_flFar.Modified();
}

void CPointShadowCamera::UpdateProjection( void )
{
	m_matProjection = glm::perspective( M_PI * 0.5f, 1.0f, m_flNear.Get(), m_flFar.Get() );
}

void CPointShadowCamera::UpdateTotal( void )
{
	for (unsigned char i = 0; i < 6; i++) m_matTotal[i] = m_matProjection * m_matView[i];
}