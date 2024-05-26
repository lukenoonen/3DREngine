#include "TestPlayer.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "TimeManager.h"
#include "PerspectivePlayerCamera.h"

bool CC_PForward( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_forward, true );
	return true;
}
CConCommand cc_pforward( "+forward", CC_PForward );

bool CC_PBack( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_back, true );
	return true;
}
CConCommand cc_pbackward( "+back", CC_PBack );

bool CC_PLeft( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_left, true );
	return true;
}
CConCommand cc_pleft( "+left", CC_PLeft );

bool CC_PRight( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_right, true );
	return true;
}
CConCommand cc_pright( "+right", CC_PRight );

bool CC_PUp( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_up, true);
	return true;
}
CConCommand cc_pup( "+up", CC_PUp );

bool CC_PDown( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_down, true );
	return true;
}
CConCommand cc_pdown( "+down", CC_PDown );

bool CC_MForward( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_forward, false );
	return true;
}
CConCommand cc_mforward( "-forward", CC_MForward );

bool CC_MBack( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_back, false );
	return true;
}
CConCommand cc_mback( "-back", CC_MBack );

bool CC_MLeft( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_left, false );
	return true;
}
CConCommand ccm_left( "-left", CC_MLeft );

bool CC_MRight( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_right, false );
	return true;
}
CConCommand cc_mright( "-right", CC_MRight );

bool CC_MUp( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_up, false );
	return true;
}
CConCommand cc_mup( "-up", CC_MUp );

bool CC_MDown( void )
{
	CTestPlayer *pTestPlayer = dynamic_cast<CTestPlayer *>(pEntityManager->GetLocalPlayer());
	if (pTestPlayer)
		pTestPlayer->SetMovement( EMovement::t_down, false );
	return true;
}
CConCommand cc_mdown( "-down", CC_MDown );

CConFloat cf_pl_speed( 10.0f, "pl_speed" );
CConVec2 cv2_pl_mousesens( g_vec2One * 0.08f, "pl_mousesens" );

DEFINE_LINKED_CLASS( CTestPlayer, player_test )

CTestPlayer::CTestPlayer()
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EMovement::i_count; i++)
		m_bMovement[i] = false;

	m_vec3Front = GetRotation() * g_vec3Front;
	m_vec3Right = GetRotation() * g_vec3Right;
	m_vec3Up = GetRotation() * g_vec3Up;

	pEntityManager->SetLocalPlayer( this );
}

bool CTestPlayer::Init( void )
{
	if (!BaseClass::Init())
		return false;

	CBasePlayerCamera *pCamera = GetCamera();

	pCamera->SetParent( this );
	pCamera->SetPosition( GetPosition() );
	pCamera->SetRotation( GetRotation() );
	pCamera->SetScale( GetScale() );

	return true;
}

void CTestPlayer::PreThink( void )
{
	glm::vec3 vec3Rotation = glm::eulerAngles( GetRotation() );

	if (pInputManager->IsCursorLocked())
	{
		glm::vec2 vec2Offset = pInputManager->GetCursorDelta();
		if (vec2Offset != g_vec2Zero)
		{
			vec3Rotation.z -= vec2Offset.x * glm::radians( cv2_pl_mousesens.GetValue().x );
			vec3Rotation.x -= vec2Offset.y * glm::radians( cv2_pl_mousesens.GetValue().y );

			if (vec3Rotation.x > glm::radians( 89.0f ))
				vec3Rotation.x = glm::radians( 89.0f );
			if (vec3Rotation.x < -glm::radians( 89.0f ))
				vec3Rotation.x = -glm::radians( 89.0f );

			SetRotation( glm::quat( vec3Rotation ) );

			m_vec3Front = GetRotation() * g_vec3Front;
			m_vec3Right = GetRotation() * g_vec3Right;
			m_vec3Up = GetRotation() * g_vec3Up;
		}
	}

	float flVelocity = cf_pl_speed.GetValue() * pTimeManager->GetFrameTime();
	if (m_bMovement[(EBaseEnum)EMovement::t_forward])
		AddPosition( m_vec3Front * flVelocity );
	if (m_bMovement[(EBaseEnum)EMovement::t_back])
		AddPosition( -m_vec3Front * flVelocity );
	if (m_bMovement[(EBaseEnum)EMovement::t_left])
		AddPosition( -m_vec3Right * flVelocity );
	if (m_bMovement[(EBaseEnum)EMovement::t_right])
		AddPosition( m_vec3Right * flVelocity );
	if (m_bMovement[(EBaseEnum)EMovement::t_up])
		AddPosition( g_vec3Up * flVelocity );
	if (m_bMovement[(EBaseEnum)EMovement::t_down])
		AddPosition( -g_vec3Up * flVelocity );

	BaseClass::PreThink();
}

void CTestPlayer::SetMovement( EMovement eMovement, bool bMoving )
{
	m_bMovement[(EBaseEnum)eMovement] = bMoving;
}