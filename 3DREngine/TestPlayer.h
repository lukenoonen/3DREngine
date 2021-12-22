#ifndef TESTPLAYER_H
#define TESTPLAYER_H

#include "Global.h"
#include "BasePlayer.h"

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

class CTestPlayer : public CBasePlayer
{
public:
	DECLARE_CLASS( CTestPlayer, CBasePlayer );

	CTestPlayer( const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale );

	virtual void PreThink( void );

	void SetMovement( Movement_t tMovement, bool bMoving );

private:
	bool m_bMovement[MOVEMENT_COUNT];

	glm::vec3 m_vecFront;
	glm::vec3 m_vecUp;
	glm::vec3 m_vecRight;
};

#endif // TESTPLAYER_H