#include "OrthoPlayerCamera.h"
#include "RenderManager.h"

DEFINE_LINKED_CLASS( COrthoPlayerCamera, camera_player_ortho )

COrthoPlayerCamera::COrthoPlayerCamera()
{

}

void COrthoPlayerCamera::PostThink( void )
{
	if (cf_r_width.WasDispatched() || cf_r_near.WasDispatched() || cf_r_far.WasDispatched())
		MarkUpdateProjection();

	BaseClass::PostThink();
}

void COrthoPlayerCamera::UpdateProjection( void )
{
	const glm::ivec2 &vec2Size = m_pFramebuffer->GetSize();
	float flWidth = cf_r_width.GetValue();
	float flHeight = flWidth * (float)vec2Size.y / (float)vec2Size.x;
	m_matProjection[0] = glm::ortho( -flWidth, flWidth, -flHeight, flHeight, cf_r_near.GetValue(), cf_r_far.GetValue() );
}