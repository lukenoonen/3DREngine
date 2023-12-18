#include "BaseColorCamera.h"
#include "FramebufferColor.h"

DEFINE_DATADESC( CBaseColorCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferColor, m_pFramebuffer, "framebuffer", 0 )

END_DATADESC()

CBaseColorCamera::CBaseColorCamera()
{
	m_pFramebuffer = new CFramebufferColor();
}