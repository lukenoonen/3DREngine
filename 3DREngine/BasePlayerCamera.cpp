#include "BasePlayerCamera.h"

CBasePlayerCamera::CBasePlayerCamera()
{
	m_bPlayerCamera = false;
}

bool CBasePlayerCamera::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_bPlayerCamera;
}

void CBasePlayerCamera::SetPlayerCamera( bool bPlayerCamera )
{
	m_bPlayerCamera = bPlayerCamera;
}