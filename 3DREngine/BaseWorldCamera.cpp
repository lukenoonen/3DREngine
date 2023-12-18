#include "BaseWorldCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

CBaseWorldCamera::CBaseWorldCamera()
{
	m_matProjection.resize( 1 );
	m_matView.resize( 1 );
	m_matTotal.resize( 1 );
	m_matTotalLocked.resize( 1 );
}

void CBaseWorldCamera::PerformRender( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 0, &m_matTotal[0] );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 1, &m_matTotalLocked[0] );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 2, &GetPosition() );

	pEntityManager->DrawEntities();
}

void CBaseWorldCamera::UpdateView( void )
{
	m_matView[0] = glm::lookAt(GetPosition(), GetPosition() + GetRotation() * g_vec3Front, GetRotation() * g_vec3Up);
}

void CBaseWorldCamera::UpdateTotal( void )
{
	m_matTotal[0] = m_matProjection[0] * m_matView[0];
	m_matTotalLocked[0] = m_matProjection[0] * glm::mat4( glm::mat3( m_matView[0] ) );
}