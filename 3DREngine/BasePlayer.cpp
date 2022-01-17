#include "BasePlayer.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "GlobalManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "CommandManager.h"

CBasePlayer::CBasePlayer( CBasePlayerCamera *pCamera )
{
	pCamera->SetPlayerCamera( true );

	m_pCamera = pCamera;
}

bool CBasePlayer::IsPlayer( void ) const
{
	return true;
}

void CBasePlayer::SetCamera( CBasePlayerCamera *pCamera )
{
	m_pCamera->SetPlayerCamera( false );
	pCamera->SetPlayerCamera( true );

	m_pCamera = pCamera;
}

CBasePlayerCamera *CBasePlayer::GetCamera( void ) const
{
	return m_pCamera;
}