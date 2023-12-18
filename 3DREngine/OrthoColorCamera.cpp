#include "OrthoColorCamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( COrthoColorCamera )

	DEFINE_FIELD( DataField, float, m_flWidth, "width", 0 )
	DEFINE_FIELD( DataField, float, m_flNear, "near", 0 )
	DEFINE_FIELD( DataField, float, m_flFar, "far", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( COrthoColorCamera, camera_color_ortho )

COrthoColorCamera::COrthoColorCamera()
{
	m_flWidth = 16.0f;
	m_flNear = -16.0f;
	m_flFar = 16.0f;
}

void COrthoColorCamera::SetWidth( float flWidth )
{
	m_flWidth = flWidth * 0.5f;
	MarkUpdateProjection();
}

void COrthoColorCamera::SetNear( float flNear )
{
	m_flNear = flNear;
	MarkUpdateProjection();
}

void COrthoColorCamera::SetFar( float flFar )
{
	m_flFar = flFar;
	MarkUpdateProjection();
}

void COrthoColorCamera::UpdateProjection( void )
{
	const glm::ivec2 &vec2Size = m_pFramebuffer->GetSize();
	float flHeight = m_flWidth * (float)vec2Size.y / (float)vec2Size.x;
	m_matProjection[0] = glm::ortho( -m_flWidth, m_flWidth, -flHeight, flHeight, m_flNear, m_flFar );
}