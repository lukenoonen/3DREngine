#include "Camera.h"
#include "GlobalManager.h"
#include "InputManager.h"

CCamera::CCamera( const glm::vec3 & vecPosition, float flYaw, float flPitch )
{
	m_vecPosition = vecPosition;
	m_flYaw = flYaw;
	m_flPitch = flPitch;

	m_vecFront = g_vecFront;

	m_flMovementSpeed = 2.5f;
	m_flMouseSensitivity = 0.1f;
	m_flZoom = 90.0f;

	UpdateCameraVectors();
}

glm::vec3 CCamera::GetPosition( void )
{
	return m_vecPosition;
}

glm::vec3 CCamera::GetFront( void )
{
	return m_vecFront;
}

glm::mat4 CCamera::GetProjectionMatrix( void )
{
	return glm::perspective( glm::radians( m_flZoom ), 1920.0f / 1080.0f, 0.001f, 100.0f );
}

glm::mat4 CCamera::GetViewMatrix( void )
{
	return glm::lookAt( m_vecPosition, m_vecPosition + m_vecFront, m_vecUp );
}

void CCamera::SetMovement( CamMovement_t tDirection, bool bMoving )
{
	m_bMovement[tDirection] = bMoving;
}

void CCamera::OnLoop( void )
{
	m_flYaw += vecOffset.x;
	m_flPitch -= vecOffset.y;

	if (m_flPitch > 89.0f)
		m_flPitch = 89.0f;
	if (m_flPitch < -89.0f)
		m_flPitch = -89.0f;

	float flVelocity = m_flMovementSpeed * pGlobalValues->GetFrameTime();
	if (m_bMovement[CAM_MOVEMENT_FORWARD])
		m_vecPosition += m_vecFront * flVelocity;
	if (m_bMovement[CAM_MOVEMENT_BACKWARD])
		m_vecPosition -= m_vecFront * flVelocity;
	if (m_bMovement[CAM_MOVEMENT_LEFT])
		m_vecPosition -= m_vecRight * flVelocity;
	if (m_bMovement[CAM_MOVEMENT_RIGHT])
		m_vecPosition += m_vecRight * flVelocity;

	glm::vec2 vecOffset = pInputManager->GetMouseDelta() * m_flMouseSensitivity;

	UpdateCameraVectors();
}

void CCamera::UpdateCameraVectors( void )
{
	glm::vec3 vecFront;
	vecFront.x = cos( glm::radians( m_flYaw ) ) * cos( glm::radians( m_flPitch ) );
	vecFront.y = -sin( glm::radians( m_flYaw ) ) * cos( glm::radians( m_flPitch ) );
	vecFront.z = sin( glm::radians( m_flPitch ) );

	m_vecFront = glm::normalize( vecFront );
	m_vecRight = glm::normalize( glm::cross( m_vecFront, g_vecUp ) );
	m_vecUp = glm::normalize( glm::cross( m_vecRight, m_vecFront ) );
}