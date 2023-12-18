#include "PerspectivePlayerCamera.h"
#include "RenderManager.h"

DEFINE_LINKED_CLASS( CPerspectivePlayerCamera, camera_player_perspective )

CPerspectivePlayerCamera::CPerspectivePlayerCamera()
{

}

void CPerspectivePlayerCamera::PostThink( void )
{
	if (cf_r_fov.WasDispatched() || cf_r_near.WasDispatched() || cf_r_far.WasDispatched())
		MarkUpdateProjection();

	BaseClass::PostThink();
}

void CPerspectivePlayerCamera::UpdateProjection( void )
{
	const glm::ivec2 &vec2Size = m_pFramebuffer->GetSize();
	m_matProjection[0] = glm::perspective( glm::radians( cf_r_fov.GetValue() ), (float)vec2Size.x / (float)vec2Size.y, cf_r_near.GetValue(), cf_r_far.GetValue() );
}