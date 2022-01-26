#include "BasePlayer.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "GlobalManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "CommandManager.h"

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
	if (m_pCamera != pCamera)
	{
		if (m_pCamera)
			m_pCamera->SetPlayerCamera( false );

		m_pCamera = pCamera;

		if (m_pCamera)
			m_pCamera->SetPlayerCamera( true );
	}
}

CBasePlayerCamera *CBasePlayer::GetCamera( void ) const
{
	return m_pCamera;
}