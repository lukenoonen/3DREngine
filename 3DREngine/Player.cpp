#include "Player.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "GlobalManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "CommandManager.h"

bool CC_PForward( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_FORWARD, true );
	return true;
}
CConCommand cc_pforward( "+forward", CC_PForward );

bool CC_PBack( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_BACK, true );
	return true;
}
CConCommand cc_pbackward( "+back", CC_PBack );

bool CC_PLeft( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_LEFT, true );
	return true;
}
CConCommand cc_pleft( "+left", CC_PLeft );

bool CC_PRight( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_RIGHT, true );
	return true;
}
CConCommand cc_pright( "+right", CC_PRight );

bool CC_PUp( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_UP, true );
	return true;
}
CConCommand cc_pup( "+up", CC_PUp );

bool CC_PDown( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_DOWN, true );
	return true;
}
CConCommand cc_pdown( "+down", CC_PDown );

bool CC_MForward( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_FORWARD, false );
	return true;
}
CConCommand cc_mforward( "-forward", CC_MForward );

bool CC_MBack( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_BACK, false );
	return true;
}
CConCommand cc_mback( "-back", CC_MBack );

bool CC_MLeft( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_LEFT, false );
	return true;
}
CConCommand ccm_left( "-left", CC_MLeft );

bool CC_MRight( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_RIGHT, false );
	return true;
}
CConCommand cc_mright( "-right", CC_MRight );

bool CC_MUp( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_UP, false );
	return true;
}
CConCommand cc_mup( "-up", CC_MUp );

bool CC_MDown( void )
{
	pEntityManager->GetPlayer()->SetMovement( MOVEMENT_DOWN, false );
	return true;
}
CConCommand cc_mdown( "-down", CC_MDown );

CConFloat cf_pl_speed( 10.0f, "pl_speed" );
CConVec2 cv2_pl_mousesens( g_vec2One * 0.08f, "pl_mousesens" );

CPlayer::CPlayer( CBasePlayerCamera *pCamera, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_pCamera = NULL;
	SetCamera( pCamera );

	for (unsigned int i = 0; i < MOVEMENT_COUNT; i++)
		m_bMovement[i] = false;

	m_vecFront = GetRotation() * g_vecFront;
	m_vecRight = GetRotation() * g_vecRight;
	m_vecUp = GetRotation() * g_vecUp;
}

void CPlayer::PreThink( void )
{
	BaseClass::PreThink();

	glm::vec3 vecRotation = glm::eulerAngles( GetRotation() );

	glm::vec2 vecOffset = pInputManager->GetMouseDelta();
	if (vecOffset != g_vec2Zero)
	{
		vecRotation.z -= vecOffset.x * glm::radians( cv2_pl_mousesens.GetValue().x );
		vecRotation.x -= vecOffset.y * glm::radians( cv2_pl_mousesens.GetValue().y );

		if (vecRotation.x > glm::radians( 89.0f ))
			vecRotation.x = glm::radians( 89.0f );
		if (vecRotation.x < -glm::radians( 89.0f ))
			vecRotation.x = -glm::radians( 89.0f );

		SetRotation( glm::quat( vecRotation ) );

		m_vecFront = GetRotation() * g_vecFront;
		m_vecRight = GetRotation() * g_vecRight;
		m_vecUp = GetRotation() * g_vecUp;
	}

	float flVelocity = cf_pl_speed.GetValue() * pGlobalValues->GetFrameTime();
	if (m_bMovement[MOVEMENT_FORWARD])
		AddPosition( m_vecFront * flVelocity );
	if (m_bMovement[MOVEMENT_BACK])
		AddPosition( -m_vecFront * flVelocity );
	if (m_bMovement[MOVEMENT_LEFT])
		AddPosition( -m_vecRight * flVelocity );
	if (m_bMovement[MOVEMENT_RIGHT])
		AddPosition( m_vecRight * flVelocity );
	if (m_bMovement[MOVEMENT_UP])
		AddPosition( g_vecUp * flVelocity );
	if (m_bMovement[MOVEMENT_DOWN])
		AddPosition( -g_vecUp * flVelocity );
}

bool CPlayer::IsPlayer( void ) const
{
	return true;
}

void CPlayer::SetMovement( Movement_t tMovement, bool bMoving )
{
	m_bMovement[tMovement] = bMoving;
}

void CPlayer::SetCamera( CBasePlayerCamera *pCamera )
{
	if (m_pCamera != pCamera)
	{
		if (m_pCamera)
			RemoveChild( m_pCamera );

		if (pCamera)
			AddChild( pCamera );

		m_pCamera = pCamera;
	}
}

CBasePlayerCamera *CPlayer::GetCamera( void ) const
{
	return m_pCamera;
}