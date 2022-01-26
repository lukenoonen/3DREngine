#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Global.h"
#include "BaseEntity.h"
#include "BasePlayer.h"
#include "BaseDrawable.h"
#include "BaseLight.h"
#include "BaseCamera.h"

class CEntityManager
{
public:
	DECLARE_CLASS_NOBASE( CEntityManager );

	CEntityManager();
	~CEntityManager();
	
	void OnLoop( void );

	void DrawEntities( void );
	void DrawUnlitEntities( void );
	void DrawLitEntities( void );

	void AddEntity( CBaseEntity *pEntity );
	void RemoveEntity( CBaseEntity *pEntity );

	void ClearEntities( void );

	CBasePlayer *GetPlayer( void );

private:
	std::vector<CBaseEntity *> m_pEntitiesToAdd;
	std::vector<CBaseEntity *> m_pEntities;
	std::vector<CBaseEntity *> m_pEntitiesToRemove;

	std::vector<CBaseLight *> m_pLightEntities;
	std::vector<CBaseCamera *> m_pCameraEntities;
	std::vector<CBaseDrawable *> m_pDrawEntities;

	CBasePlayer *m_pPlayer;
};

#endif // ENTITYMANAGER_H