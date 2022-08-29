#ifndef PLAYER_H
#define PLAYER_H

#include "Global.h"
#include "BaseTransform.h"
#include "CommandManager.h"
#include "BasePlayerCamera.h"

class CBasePlayer : public CBaseTransform
{
public:
	DECLARE_CLASS( CBasePlayer, CBaseTransform )

	CBasePlayer();

	virtual bool IsPlayer( void ) const;

	void SetCamera( CBasePlayerCamera *pCamera );
	CBasePlayerCamera *GetCamera( void ) const;

private:

	CBasePlayerCamera *m_pCamera;
};

#endif // PLAYER_H