#include "Player.h"
#include "EntityManager.h"
#include "InputManager.h"
#include "GlobalManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"

CConFloat cf_r_fov( "r_fov", 90.0f );
CConFloat cf_r_near( "r_near", 0.1f );
CConFloat cf_r_far( "r_far", 100000000000.0f );

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

CConFloat cf_pl_speed( "pl_speed", 10.0f );
CConVec2 cv2_pl_mousesens( "pl_mousesens", g_vec2One * 0.08f );

CPlayer::CPlayer( const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	for (unsigned int i = 0; i < MOVEMENT_COUNT; i++)
		m_bMovement[i] = false;

	m_vecFront = GetRotation() * g_vecFront;
	m_vecRight = glm::normalize( glm::cross( m_vecFront, g_vecUp ) );
	m_vecUp = glm::normalize( glm::cross( m_vecRight, m_vecFront ) );

	m_flPrevWindowSizeRatio = 0.0f;

	m_bProjectionChanged = false;
	m_bViewChanged = false;
}

void CPlayer::Think( void )
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

void CPlayer::PostThink( void )
{
	float flWindowSizeRatio = pRenderManager->GetWindowSizeRatio();
	if (m_flPrevWindowSizeRatio != flWindowSizeRatio || cf_r_fov.WasDispatched() || cf_r_near.WasDispatched() || cf_r_far.WasDispatched())
	{
		m_matProjection = glm::perspective( glm::radians( cf_r_fov.GetValue() ), flWindowSizeRatio, cf_r_near.GetValue(), cf_r_far.GetValue() );
		m_flPrevWindowSizeRatio = flWindowSizeRatio;

		m_bProjectionChanged = true;
	}
	else
	{
		m_bProjectionChanged = false;
	}

	if (HasTransformUpdated())
	{
		m_vecFront = GetRotation() * g_vecFront;
		m_vecRight = glm::normalize( glm::cross( m_vecFront, g_vecUp ) );
		m_vecUp = glm::normalize( glm::cross( m_vecRight, m_vecFront ) );
		m_matView = glm::lookAt( GetPosition(), GetPosition() + m_vecFront, m_vecUp );
		pShaderManager->SetUniformBufferObject( UBO_VIEW, 2, &GetPosition() );

		m_bViewChanged = true;

		ResetTransformUpdated();
	}
	else
	{
		m_bViewChanged = false;
	}

	if (m_bViewChanged || m_bProjectionChanged)
	{
		pShaderManager->SetUniformBufferObject( UBO_VIEW, 0, &(m_matProjection * m_matView) );
		pShaderManager->SetUniformBufferObject( UBO_VIEW, 1, &(m_matProjection * glm::mat4( glm::mat3( m_matView ) )) );
	}
}

void CPlayer::SetMovement( Movement_t tMovement, bool bMoving )
{
	m_bMovement[tMovement] = bMoving;
}

bool CPlayer::IsPlayer( void ) const
{
	return true;
}

const glm::vec3 &CPlayer::GetFront( void ) const
{
	return m_vecFront;
}

const glm::vec3 &CPlayer::GetUp( void ) const
{
	return m_vecUp;
}

const glm::vec3 &CPlayer::GetRight( void ) const
{
	return m_vecRight;
}

const glm::mat4 &CPlayer::GetProjection( void ) const
{
	return m_matProjection;
}

const glm::mat4 &CPlayer::GetView( void ) const
{
	return m_matView;
}

bool CPlayer::HasProjectionChanged( void ) const
{
	return m_bProjectionChanged;
}

bool CPlayer::HasViewChanged( void ) const
{
	return m_bViewChanged;
}