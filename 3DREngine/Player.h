#ifndef PLAYER_H
#define PLAYER_H

#include "Global.h"
#include "BaseTransform.h"
#include "CommandManager.h"
#include "BasePlayerCamera.h"

enum Movement_t : unsigned char
{
	MOVEMENT_FORWARD = 0,
	MOVEMENT_BACK,
	MOVEMENT_LEFT,
	MOVEMENT_RIGHT,
	MOVEMENT_UP,
	MOVEMENT_DOWN,
	MOVEMENT_COUNT,
};

class CPlayer : public CBaseTransform
{
public:
	DECLARE_CLASS( CPlayer, CBaseTransform );

	CPlayer( CBasePlayerCamera *pCamera, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PreThink( void );

	virtual bool IsPlayer( void ) const;

	void SetMovement( Movement_t tMovement, bool bMoving );

	void SetCamera( CBasePlayerCamera *pCamera );
	CBasePlayerCamera *GetCamera( void ) const;

private:
	bool m_bMovement[MOVEMENT_COUNT];

	CBasePlayerCamera *m_pCamera;

	glm::vec3 m_vecFront;
	glm::vec3 m_vecUp;
	glm::vec3 m_vecRight;
};

#endif // PLAYER_H