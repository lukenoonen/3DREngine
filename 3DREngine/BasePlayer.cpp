#include "BasePlayer.h"

DEFINE_DATADESC( CBasePlayer )

	DEFINE_FIELD( LinkedDataField, CHandle<CBasePlayerCamera>, m_hCamera, "camera", FL_REQUIRED )

END_DATADESC()

CBasePlayer::CBasePlayer()
{

}

bool CBasePlayer::IsPlayer( void ) const
{
	return true;
}

bool CBasePlayer::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (!m_hCamera)
		return false;

	return true;
}

void CBasePlayer::SetCamera( CBasePlayerCamera *pCamera )
{
	m_hCamera = pCamera;
}

CBasePlayerCamera *CBasePlayer::GetCamera( void ) const
{
	return m_hCamera;
}