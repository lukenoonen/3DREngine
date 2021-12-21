#include "BaseUniformMSAACamera.h"
#include "RenderManager.h"

CBaseUniformMSAACamera::CBaseUniformMSAACamera( unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( (unsigned int)ci_r_msaalevel.GetValue(), cv_r_windowsize.GetValue(), uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{

}

void CBaseUniformMSAACamera::PreThink( void )
{
	if (cv_r_windowsize.WasDispatched())
		SetSize( cv_r_windowsize.GetValue() );

	if (ci_r_msaalevel.WasDispatched())
		SetMSAALevel( (unsigned int)ci_r_msaalevel.GetValue() );
}