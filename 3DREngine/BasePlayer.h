#ifndef PLAYER_H
#define PLAYER_H

#include "Global.h"
#include "BaseTransform.h"
#include "CommandManager.h"
#include "BasePlayerCamera.h"

class CBasePlayer : public CBaseTransform
{
public:
	DECLARE_CLASS( CBasePlayer, CBaseTransform );

	CBasePlayer( CBasePlayerCamera *pCamera, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual bool IsPlayer( void ) const;

	void SetCamera( CBasePlayerCamera *pCamera );
	CBasePlayerCamera *GetCamera( void ) const;

private:

	CBasePlayerCamera *m_pCamera;
};

#endif // PLAYER_H