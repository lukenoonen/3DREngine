#include "BaseUniformSizeCamera.h"
#include "RenderManager.h"

CBaseUniformSizeCamera::CBaseUniformSizeCamera( unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( cv_r_windowsize.GetValue(), uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{

}

void CBaseUniformSizeCamera::PreThink( void )
{
	if (cv_r_windowsize.WasDispatched())
		SetSize( cv_r_windowsize.GetValue() );
}