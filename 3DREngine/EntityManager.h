#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Global.h"
#include "BaseEntity.h"
#include "BasePlayer.h"

class CEntityManager
{
public:
	DECLARE_CLASS_NOBASE( CEntityManager );

	CEntityManager();
	~CEntityManager();

	void Reset( void );
	
	void OnLoop( void );

	void AddEntity( CBaseEntity *pNewEntity );
	void RemoveEntity( CBaseEntity *pEntity );
	void ClearEntities( void );

	CBasePlayer *GetPlayer( void );

private:
	std::vector<CBaseEntity *> m_pEntities;
	CBasePlayer *m_pPlayer;
};

#endif // ENTITYMANAGER_H