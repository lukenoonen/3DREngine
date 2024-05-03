#include "PerspectivePlayerCamera.h"
#include "RenderManager.h"

DEFINE_LINKED_CLASS( CPerspectivePlayerCamera, camera_player_perspective )

CPerspectivePlayerCamera::CPerspectivePlayerCamera()
{

}

bool CPerspectivePlayerCamera::ShouldUpdateProjection( void ) const
{
	return cv_r_windowsize.WasDispatched() || cf_r_fov.WasDispatched() || cf_r_near.WasDispatched() || cf_r_far.WasDispatched();
}

glm::mat4 CPerspectivePlayerCamera::CalculateProjection( void ) const
{
	const glm::ivec2 &vec2Size = cv_r_windowsize.GetValue();
	return glm::perspective( glm::radians( cf_r_fov.GetValue() ), (float)vec2Size.x / (float)vec2Size.y, cf_r_near.GetValue(), cf_r_far.GetValue() );
}