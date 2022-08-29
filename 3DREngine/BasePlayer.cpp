#include "BasePlayer.h"

CBasePlayer::CBasePlayer()
{
	m_pCamera = NULL;
}

bool CBasePlayer::IsPlayer( void ) const
{
	return true;
}

void CBasePlayer::SetCamera( CBasePlayerCamera *pCamera )
{
	m_pCamera = pCamera;
}

CBasePlayerCamera *CBasePlayer::GetCamera( void ) const
{
	return m_pCamera;
}