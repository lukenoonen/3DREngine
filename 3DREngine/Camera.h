#ifndef CAMERA_H
#define CAMERA_H

#include "Global.h"

enum CamMovement_t
{
	CAM_MOVEMENT_FORWARD = 0,
	CAM_MOVEMENT_BACKWARD,
	CAM_MOVEMENT_LEFT,
	CAM_MOVEMENT_RIGHT,
	CAM_MOVEMENT_COUNT,
};

class CCamera
{
public:
	DECLARE_CLASS_NOBASE( CCamera );

	CCamera();

	glm::vec3 GetPosition( void );
	glm::vec3 GetFront( void );
	glm::mat4 GetViewMatrix( void );
	glm::mat4 GetProjectionMatrix( void );
	void SetMovement( CamMovement_t tDirection, bool bMoving );

	void OnLoop( void );

private:
	void UpdateCameraVectors( void );

	glm::vec3 m_vecPosition;
	glm::vec3 m_vecUp;
	glm::vec3 m_vecRight;
	glm::vec3 m_vecFront;

	float m_flYaw;
	float m_flPitch;

	float m_flMovementSpeed;
	float m_flMouseSensitivity;
	float m_flZoom;

	bool m_bMovement[CAM_MOVEMENT_COUNT];

	glm::mat4 m_matViewMatrix;
	glm::mat4 m_matProjectionMatrix;
};

#endif // CAMERA_H