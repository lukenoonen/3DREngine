#include "PlanarReflectionCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_LINKED_CLASS( CPlanarReflectionCamera, camera_reflection_planar )

CPlanarReflectionCamera::CPlanarReflectionCamera()
{

}

int CPlanarReflectionCamera::BindTexture( void )
{
	return BaseClass::BindTexture();
}

bool CPlanarReflectionCamera::ShouldFlipPortal( void )
{
	return true;
}

void CPlanarReflectionCamera::UpdateTransform( void )
{
	m_matTransform = glm::mat4(
		-2.0f * m_vec4Plane.x * m_vec4Plane.x + 1.0f, -2.0f * m_vec4Plane.y * m_vec4Plane.x, -2.0f * m_vec4Plane.z * m_vec4Plane.x, 0.0f,
		-2.0f * m_vec4Plane.x * m_vec4Plane.y, -2.0f * m_vec4Plane.y * m_vec4Plane.y + 1.0f, -2.0f * m_vec4Plane.z * m_vec4Plane.y, 0.0f,
		-2.0f * m_vec4Plane.x * m_vec4Plane.z, -2.0f * m_vec4Plane.y * m_vec4Plane.z, -2.0f * m_vec4Plane.z * m_vec4Plane.z + 1.0f, 0.0f,
		-2.0f * m_vec4Plane.x * m_vec4Plane.w, -2.0f * m_vec4Plane.y * m_vec4Plane.w, -2.0f * m_vec4Plane.z * m_vec4Plane.w, 1.0f
	);
}