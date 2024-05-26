#include "BaseColorCamera.h"

DEFINE_DATADESC( CBaseColorCamera )

	DEFINE_FIELD( EmbeddedDataField, CFramebufferColor, m_fFramebufferColor, "framebuffer", FL_NONE )

END_DATADESC()

CBaseColorCamera::CBaseColorCamera()
{

}

CBaseFramebuffer *CBaseColorCamera::GetFramebuffer( void )
{
	return &m_fFramebufferColor;
}

const CBaseFramebuffer *CBaseColorCamera::GetFramebuffer( void ) const
{
	return &m_fFramebufferColor;
}