#include "OrthoColorCamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( COrthoColorCamera )

	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flWidth, "width", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flNear, "near", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flFar, "far", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( COrthoColorCamera, camera_world_ortho )

COrthoColorCamera::COrthoColorCamera()
{
	m_flWidth = 16.0f;
	m_flNear = -16.0f;
	m_flFar = 16.0f;
}

void COrthoColorCamera::SetWidth( float flWidth )
{
	m_flWidth = flWidth * 0.5f;
}

void COrthoColorCamera::SetNear( float flNear )
{
	m_flNear = flNear * 0.5f;
}

void COrthoColorCamera::SetFar( float flFar )
{
	m_flFar = flFar * 0.5f;
}

bool COrthoColorCamera::ShouldUpdateProjection( void ) const
{
	return m_flWidth.Modified() || m_flNear.Modified() || m_flFar.Modified();
}

glm::mat4 COrthoColorCamera::CalculateProjection( void ) const
{
	const glm::ivec2 &vec2Size = GetFramebuffer()->GetSize();
	float flHeight = m_flWidth.Get() * (float)vec2Size.y / (float)vec2Size.x;
	return glm::ortho( -m_flWidth.Get(), m_flWidth.Get(), -flHeight, flHeight, m_flNear.Get(), m_flFar.Get() );
}