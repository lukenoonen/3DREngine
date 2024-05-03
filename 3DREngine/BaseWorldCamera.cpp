#include "BaseWorldCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

CBaseWorldCamera::CBaseWorldCamera()
{

}

const glm::mat4 &CBaseWorldCamera::GetView( void ) const
{
	return m_matView;
}

const glm::mat4 &CBaseWorldCamera::GetProjection( void ) const
{
	return m_matProjection;
}

const glm::mat4 &CBaseWorldCamera::GetTotal( void ) const
{
	return m_matTotal;
}

void CBaseWorldCamera::PerformRender( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 0, &m_matTotal );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 1, &m_matTotalLocked );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 2, &GetPosition() );

	pEntityManager->DrawEntities();
}

bool CBaseWorldCamera::ShouldUpdateView( void ) const
{
	return PositionUpdated() || RotationUpdated();
}

void CBaseWorldCamera::UpdateView( void )
{
	m_matView = CalculateView();
}

void CBaseWorldCamera::UpdateProjection( void )
{
	m_matProjection = CalculateProjection();
}

void CBaseWorldCamera::UpdateTotal( void )
{
	m_matTotal = m_matProjection * m_matView;
	m_matTotalLocked = m_matProjection * glm::mat4( glm::mat3( m_matView ) );
}

glm::mat4 CBaseWorldCamera::CalculateView( void ) const
{
	return glm::lookAt( GetPosition(), GetPosition() + GetRotation() * g_vec3Front, g_vec3Up );
}