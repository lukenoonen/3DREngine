#include "BaseUniformMSAACamera.h"
#include "RenderManager.h"

CBaseUniformMSAACamera::CBaseUniformMSAACamera()
{
	SetSize( cv_r_windowsize.GetValue() );

	SetMSAALevel( (unsigned char)ci_r_msaalevel.GetValue() );
}

void CBaseUniformMSAACamera::PostThink( void )
{
	if (cv_r_windowsize.WasDispatched())
		SetSize( cv_r_windowsize.GetValue() );

	if (ci_r_msaalevel.WasDispatched())
		SetMSAALevel( (unsigned char)ci_r_msaalevel.GetValue() );

	BaseClass::PostThink();
}