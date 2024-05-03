#include "BaseColorCamera.h"
#include "FramebufferColor.h"

DEFINE_DATADESC( CBaseColorCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferColor, m_pFramebuffer, "framebuffer", FL_NONE )

END_DATADESC()

CBaseColorCamera::CBaseColorCamera()
{
	InitFramebuffer( new CFramebufferColor() );
}