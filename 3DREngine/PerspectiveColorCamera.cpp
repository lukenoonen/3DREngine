#include "PerspectiveColorCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_DATADESC( CPerspectiveColorCamera )

	DEFINE_FIELD( DataField, float, m_flFOV, "fov", 0 )
	DEFINE_FIELD( DataField, float, m_flNear, "near", 0 )
	DEFINE_FIELD( DataField, float, m_flFar, "far", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CPerspectiveColorCamera, camera_color_perspective )

CPerspectiveColorCamera::CPerspectiveColorCamera()
{
	m_flFOV = M_PI * 0.5f;
	m_flNear = 0.1f;
	m_flFar = 1000.0f;
}

void CPerspectiveColorCamera::SetFOV( float flFOV )
{
	m_flFOV = flFOV * 0.5f;
	MarkUpdateProjection();
}

void CPerspectiveColorCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	MarkUpdateProjection();
}

void CPerspectiveColorCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	MarkUpdateProjection();
}

void CPerspectiveColorCamera::UpdateProjection( void )
{
	const glm::ivec2 &vec2Size = m_pFramebuffer->GetSize();
	m_matProjection[0] = glm::perspective( m_flFOV, (float)vec2Size.x / (float)vec2Size.y, m_flNear, m_flFar );
}