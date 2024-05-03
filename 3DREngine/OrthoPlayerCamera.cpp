#include "OrthoPlayerCamera.h"
#include "RenderManager.h"

DEFINE_LINKED_CLASS( COrthoPlayerCamera, camera_player_ortho )

COrthoPlayerCamera::COrthoPlayerCamera()
{

}

bool COrthoPlayerCamera::ShouldUpdateProjection( void ) const
{
	return cv_r_windowsize.WasDispatched() || cf_r_width.WasDispatched() || cf_r_near.WasDispatched() || cf_r_far.WasDispatched();
}

glm::mat4 COrthoPlayerCamera::CalculateProjection( void ) const
{
	const glm::ivec2 &vec2Size = cv_r_windowsize.GetValue();
	float flWidth = cf_r_width.GetValue();
	float flHeight = flWidth * (float)vec2Size.y / (float)vec2Size.x;
	return glm::ortho( -flWidth, flWidth, -flHeight, flHeight, cf_r_near.GetValue(), cf_r_far.GetValue() );
}