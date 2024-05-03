#include "PerspectiveColorCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_DATADESC( CPerspectiveColorCamera )

	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flFOV, "fov", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flNear, "near", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flFar, "far", FL_NONE )

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
}

void CPerspectiveColorCamera::SetNear( float flNear )
{
	m_flNear = flNear;
}

void CPerspectiveColorCamera::SetFar( float flFar )
{
	m_flFar = flFar;
}

bool CPerspectiveColorCamera::ShouldUpdateProjection( void ) const
{
	return m_flFOV.Modified() || m_flNear.Modified() || m_flFar.Modified();
}

glm::mat4 CPerspectiveColorCamera::CalculateProjection( void ) const
{
	const glm::ivec2 &vec2Size = m_pFramebuffer->GetSize();
	return glm::perspective( m_flFOV.Get(), (float)vec2Size.x / (float)vec2Size.y, m_flNear.Get(), m_flFar.Get() );
}