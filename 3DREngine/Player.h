#ifndef PLAYER_H
#define PLAYER_H

#include "Global.h"
#include "BaseTransform.h"

extern CConFloat cf_r_fov;
extern CConFloat cf_r_near;
extern CConFloat cf_r_far;

enum Movement_t
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

	CPlayer( const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void Think( void );
	virtual void PostThink( void );

	virtual bool IsPlayer( void ) const;

	void SetMovement( Movement_t tMovement, bool bMoving );

	const glm::vec3 &GetFront( void ) const;
	const glm::vec3 &GetUp( void ) const;
	const glm::vec3 &GetRight( void ) const;

	const glm::mat4 &GetProjection( void ) const;
	const glm::mat4 &GetView( void ) const;

	bool HasProjectionChanged( void ) const;
	bool HasViewChanged( void ) const;

private:
	bool m_bMovement[MOVEMENT_COUNT];

	glm::vec3 m_vecFront;
	glm::vec3 m_vecUp;
	glm::vec3 m_vecRight;

	float m_flPrevWindowSizeRatio;

	glm::mat4 m_matProjection;
	glm::mat4 m_matView;

	bool m_bProjectionChanged;
	bool m_bViewChanged;
};

#endif // PLAYER_H