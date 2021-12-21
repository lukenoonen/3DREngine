#include "BasePlayerCamera.h"

CBasePlayerCamera::CBasePlayerCamera( unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
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