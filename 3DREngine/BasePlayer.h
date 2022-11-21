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

	DECLARE_DATADESC()

	CBasePlayer();

	virtual bool Init( void );

	virtual bool IsPlayer( void ) const;

	void SetCamera( CBasePlayerCamera *pCamera );
	CBasePlayerCamera *GetCamera( void ) const;

private:

	CHandle<CBasePlayerCamera> m_hCamera;
};

#endif // PLAYER_H