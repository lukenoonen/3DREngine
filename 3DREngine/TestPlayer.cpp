#include "TestPlayer.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "GlobalManager.h"
#include "PerspectivePlayerCamera.h"

bool CC_PForward( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_FORWARD, true );
	return true;
}
CConCommand cc_pforward( "+forward", CC_PForward );

bool CC_PBack( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_BACK, true );
	return true;
}
CConCommand cc_pbackward( "+back", CC_PBack );

bool CC_PLeft( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_LEFT, true );
	return true;
}
CConCommand cc_pleft( "+left", CC_PLeft );

bool CC_PRight( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_RIGHT, true );
	return true;
}
CConCommand cc_pright( "+right", CC_PRight );

bool CC_PUp( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_UP, true );
	return true;
}
CConCommand cc_pup( "+up", CC_PUp );

bool CC_PDown( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_DOWN, true );
	return true;
}
CConCommand cc_pdown( "+down", CC_PDown );

bool CC_MForward( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_FORWARD, false );
	return true;
}
CConCommand cc_mforward( "-forward", CC_MForward );

bool CC_MBack( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_BACK, false );
	return true;
}
CConCommand cc_mback( "-back", CC_MBack );

bool CC_MLeft( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_LEFT, false );
	return true;
}
CConCommand ccm_left( "-left", CC_MLeft );

bool CC_MRight( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_RIGHT, false );
	return true;
}
CConCommand cc_mright( "-right", CC_MRight );

bool CC_MUp( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_UP, false );
	return true;
}
CConCommand cc_mup( "-up", CC_MUp );

bool CC_MDown( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( MOVEMENT_DOWN, false );
	return true;
}
CConCommand cc_mdown( "-down", CC_MDown );

CConFloat cf_pl_speed( 10.0f, "pl_speed" );
CConVec2 cv2_pl_mousesens( g_vec2One * 0.08f, "pl_mousesens" );

CTestPlayer::CTestPlayer() : BaseClass( new CPerspectivePlayerCamera( 1 ) )
{
	CBaseCamera *pCamera = GetCamera();
	pEntityManager->AddEntity( pCamera );

	pCamera->SetParent( this );
	pCamera->SetPosition( GetPosition() );
	pCamera->SetRotation( GetRotation() );
	pCamera->SetScale( GetScale() );

	for (unsigned int i = 0; i < MOVEMENT_COUNT; i++)
		m_bMovement[i] = false;

	m_vecFront = GetRotation() * g_vecFront;
	m_vecRight = GetRotation() * g_vecRight;
	m_vecUp = GetRotation() * g_vecUp;
}

void CTestPlayer::PreThink( void )
{
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

void CTestPlayer::SetMovement( Movement_t tMovement, bool bMoving )
{
	m_bMovement[tMovement] = bMoving;
}