#ifndef TESTPLAYER_H
#define TESTPLAYER_H

#include "Global.h"
#include "BasePlayer.h"

enum class EMovement : EBaseEnum
{
	t_forward = 0,
	t_back,
	t_left,
	t_right,
	t_up,
	t_down,

	i_count,
	i_invalid = i_count,
};

class CTestPlayer : public CBasePlayer
{
public:
	DECLARE_CLASS( CTestPlayer, CBasePlayer )

	CTestPlayer();

	virtual void PreThink( void );

	void SetMovement( EMovement eMovement, bool bMoving );

private:
	bool m_bMovement[(EBaseEnum)EMovement::i_count];

	glm::vec3 m_vec3Front;
	glm::vec3 m_vec3Up;
	glm::vec3 m_vec3Right;
};

#endif // TESTPLAYER_H