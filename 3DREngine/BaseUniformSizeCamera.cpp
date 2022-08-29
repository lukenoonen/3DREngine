#include "BaseUniformSizeCamera.h"
#include "RenderManager.h"

CBaseUniformSizeCamera::CBaseUniformSizeCamera()
{
	SetSize( cv_r_windowsize.GetValue() );
}

void CBaseUniformSizeCamera::PostThink( void )
{
	if (cv_r_windowsize.WasDispatched())
		SetSize( cv_r_windowsize.GetValue() );

	BaseClass::PostThink();
}