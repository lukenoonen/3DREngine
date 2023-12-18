#include "PointShadowCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"
#include "FramebufferShadowCubemap.h"

DEFINE_DATADESC( CPointShadowCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferShadowCubemap, m_pFramebuffer, "framebuffer", 0 )

	DEFINE_FIELD( DataField, float, m_flNear, "near", 0 )
	DEFINE_FIELD( DataField, float, m_flFar, "far", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CPointShadowCamera, camera_shadow_point )

CPointShadowCamera::CPointShadowCamera()
{
	m_pFramebuffer = new CFramebufferShadowCubemap();

	m_flNear = 0.1f;
	m_flFar = 1000.0f;

	m_matView.resize( 6 );
	m_matProjection.resize( 1 );
	m_matTotal.resize( 6 );
}

void CPointShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 6, &m_matTotal[0] );

	BaseClass::ActivateLight();
}

void CPointShadowCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	MarkUpdateProjection();
}

void CPointShadowCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	MarkUpdateProjection();
}

void CPointShadowCamera::SetBlurRadius( float flBlurRadius )
{
	SetBlurScale( flBlurRadius / M_SQRT2 );
}

void CPointShadowCamera::PerformRender( void )
{
	pRenderManager->SetRenderPass( ERenderPass::t_shadowpoint );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadow, 0, 0, 6, &m_matTotal[0] );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightmaxdistance, 0, &m_flFar );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightposition, 0, &GetPosition() );

	pEntityManager->DrawUnlitEntities();
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

void CPointShadowCamera::UpdateProjection( void )
{
	m_matProjection[0] = glm::perspective( M_PI * 0.5f, 1.0f, m_flNear, m_flFar );
}

void CPointShadowCamera::UpdateTotal( void )
{
	for (unsigned char i = 0; i < 6; i++)
		m_matTotal[i] = m_matProjection[0] * m_matView[i];
}

bool CPointShadowCamera::ShouldUpdateView( void )
{
	return PositionUpdated();
}