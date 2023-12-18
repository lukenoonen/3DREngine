#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Global.h"
#include "EntityFactory.h"
#include "EntityFlag.h"

class CBaseEntity;
class CBasePlayer;
class CBaseDrawable;
class CBaseLight;
class CBaseCamera;

class CEntityManager
{
public:
	DECLARE_CLASS_NOBASE( CEntityManager )

	CEntityManager();
	~CEntityManager();
	
	void OnLoop( void );

	void DrawEntities( void );
	void DrawUnlitEntities( void );
	void DrawLitEntities( void );

	void AddEntity( CBaseEntity *pEntity );
	void RemoveEntity( CBaseEntity *pEntity );

	void ClearEntities( void );

	// TODO: fix this, figure out a good way to handle the case where there are multiple entities of the same name
	// TODO: also remember that a load group should only look through itself for entities
	CBaseEntity *GetEntityByName( const char *sName );
	CBaseEntity *GetEntityByFileName( const char *sFileName );
	CBaseEntity *GetEntityByMapName( const char *sMapName );

	CBaseEntity *GetEntityByIndex( unsigned int uiIndex );
	unsigned int GetEntityIndex( CBaseEntity *pEntity );

	CBaseCamera *GetCurrentCamera( void ) const;
	CBaseLight *GetCurrentLight( void ) const;

	// TODO: figure out a better solution to this
	CBasePlayer *GetPlayer( unsigned int uiIndex );

	static void AddEntityFactory( CBaseEntityFactory *pEntityFactory ); // TODO: Consider cleaning this up

	bool AddEntityTest( const char *sMapName, const CTextBlock *pTextBlock );

	static void AddFlag( CEntityFlag *pEntityFlag );

	int GetFlag( const char *sKey ) const;

private:
	void AddCamera( CBaseCamera *pCamera );
	void AddDrawable( CBaseDrawable *pDrawable );
	void AddLight( CBaseLight *pLight );
	void AddPlayer( CBasePlayer *pPlayer );

	CBaseEntity *CreateEntity( const char *sMapName );
	CBaseEntity *CreateEntity( unsigned int uiEntityIndex );

	CBaseEntity *LoadEntity( const char *sFileName );

private:
	std::vector<CBaseEntity *> m_pEntities;
	unsigned int m_uiEntityCount;

	std::vector<CBaseEntity *> m_pEntitiesToRemove;

	// TODO: maybe phase these out?
	std::vector<CBaseCamera *> m_pCameraEntities;
	std::vector<CBaseDrawable *> m_pDrawableEntities;
	std::vector<CBaseLight *> m_pLightEntities;
	std::vector<CBasePlayer *> m_pPlayerEntities;

	CBaseCamera *m_pCurrentCamera;
	CBaseLight *m_pCurrentLight;

	static std::vector<CBaseEntityFactory *> *s_pEntityFactories;
	std::vector<CBaseEntityFactory *> *m_pEntityFactories;

	static std::vector<CEntityFlag *> *s_pEntityFlags; // TODO: optimise the use of vector vs other things (not just this)
	std::vector<CEntityFlag *> *m_pEntityFlags;
};

#endif // ENTITYMANAGER_H