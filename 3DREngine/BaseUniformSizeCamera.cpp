#include "BaseUniformSizeCamera.h"
#include "RenderManager.h"

CBaseUniformSizeCamera::CBaseUniformSizeCamera( unsigned int uiRenderPriority ) : BaseClass( cv_r_windowsize.GetValue(), uiRenderPriority )
{

}

void CBaseUniformSizeCamera::PreThink( void )
{
	if (cv_r_windowsize.WasDispatched())
		SetSize( cv_r_windowsize.GetValue() );
}