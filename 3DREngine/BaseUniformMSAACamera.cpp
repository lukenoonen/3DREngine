#include "BaseUniformMSAACamera.h"
#include "RenderManager.h"

CBaseUniformMSAACamera::CBaseUniformMSAACamera( unsigned int uiRenderPriority ) : BaseClass( (unsigned int)ci_r_msaalevel.GetValue(), cv_r_windowsize.GetValue(), uiRenderPriority )
{

}

void CBaseUniformMSAACamera::PreThink( void )
{
	if (cv_r_windowsize.WasDispatched())
		SetSize( cv_r_windowsize.GetValue() );

	if (ci_r_msaalevel.WasDispatched())
		SetMSAALevel( (unsigned int)ci_r_msaalevel.GetValue() );
}