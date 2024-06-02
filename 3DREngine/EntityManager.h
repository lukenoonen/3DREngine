#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Global.h"
#include "EntityFactory.h"
#include "EntityFlag.h"
#include "KeyValues.h"

class CBaseEntity;
class CBasePlayer;
class CBaseDrawable;
class CBaseLight;
class CBaseCamera;

class CEntityLoadGroup
{
public:
	DECLARE_CLASS_NOBASE( CEntityLoadGroup )

	CEntityLoadGroup();
	~CEntityLoadGroup();

	void PreThink( void );
	void Think( void );
	void PostThink( void );

	void PreRender( void );
	void Render( void );

	void ProcessRemovedEntities( void );

	void DrawUnlitEntities( void );

	void DrawLitEntities( void );

	CBaseEntity *GetEntityByName( const char *sName ) const;
	CBaseEntity *GetEntityByIndex( unsigned int uiIndex ) const;

	unsigned int GetEntityIndex( const CBaseEntity *pEntity ) const;

	void AddEntity( CBaseEntity *pEntity );
	void Flush( void );

	void RemoveEntity( CBaseEntity *pEntity );

	void ChangeLoadGroup( CBaseEntity *pEntity );

private:
	bool RemoveEntityImmediate( CBaseEntity *pEntity );

	void AddCamera( CBaseCamera *pCamera );
	void AddDrawable( CBaseDrawable *pDrawable );
	void AddLight( CBaseLight *pLight );

private:
	std::vector<CBaseEntity *> m_pEntities;
	unsigned int m_uiEntityCount;

	std::vector<CBaseEntity *> m_pEntitiesToRemove;

	std::vector<CBaseCamera *> m_pCameraEntities;
	std::vector<CBaseDrawable *> m_pDrawableEntities;
	std::vector<CBaseLight *> m_pLightEntities;
};

class CEntityManager
{
public:
	DECLARE_CLASS_NOBASE( CEntityManager )

	CEntityManager();
	~CEntityManager();
	
	void OnLoop( void );

	void PreThink( void );
	void Think( void );
	void PostThink( void );

	void PreRender( void );
	void Render( void );

	void ProcessRemovedEntities( void );

	void DrawEntities( void );
	void DrawUnlitEntities( void );
	void DrawLitEntities( void );

	void Clear( void );
	bool IsClearing( void ) const;

	void CreateLoadGroup( void );

	CBaseEntity *AddEntity( const char *sMapName, const CTextBlock *pTextBlock );
	CBaseEntity *AddEntity( const char *sMapName, const CTextBlock *pTextBlock, CEntityLoadGroup *pLoadGroup );

	template <class T> T *AddEntity( const CKeyValues *pKeyValues );
	template <class T> T *AddEntity( const CKeyValues *pKeyValues, CEntityLoadGroup *pLoadGroup );

	void FlushLoadGroup( void );
	void FlushLoadGroup( CEntityLoadGroup *pLoadGroup );

	CBaseEntity *GetEntityByName( const char *sName ) const;
	CBaseEntity *GetEntityByIndex( unsigned int uiIndex ) const;

	void SetLocalPlayer( CBasePlayer *pPlayer );
	CBasePlayer *GetLocalPlayer( void ) const;

	void SetCurrentCamera( CBaseCamera *pCamera );
	CBaseCamera *GetCurrentCamera( void ) const;

	void SetCurrentLight( CBaseLight *pLight );
	CBaseLight *GetCurrentLight( void ) const;

	static void AddEntityFactory( CBaseEntityFactory *pEntityFactory ); // TODO: Consider cleaning this up
	static void AddFlag( CEntityFlag *pEntityFlag );

	int GetFlag( const char *sKey ) const;

private:
	void AddEntity( CBaseEntity *pEntity );
	void AddEntity( CBaseEntity *pEntity, CEntityLoadGroup *pLoadGroup );

	CBaseEntity *CreateEntity( unsigned int uiEntityIndex );
	CBaseEntity *CreateEntity( const char *sMapName, const CTextBlock *pTextBlock );
	template <class T> T *CreateEntity( const CKeyValues *pKeyValues );

	CBaseEntity *LoadEntity( const char *sFileName );

private:
	bool m_bClearing;

	std::vector<CEntityLoadGroup> m_LoadGroups;
	CEntityLoadGroup *m_pGlobalLoadGroup;
	CEntityLoadGroup *m_pActiveLoadGroup;

	CBasePlayer *m_pLocalPlayer;
	CBaseCamera *m_pCurrentCamera;
	CBaseLight *m_pCurrentLight;

	static std::vector<CBaseEntityFactory *> *s_pEntityFactories;
	std::vector<CBaseEntityFactory *> *m_pEntityFactories;

	static std::vector<CEntityFlag *> *s_pEntityFlags; // TODO: optimise the use of vector vs other things (not just this)
	std::vector<CEntityFlag *> *m_pEntityFlags;
};

template <class T> T *CEntityManager::AddEntity( const CKeyValues *pKeyValues )
{
	T *pEntity = CreateEntity<T>( pKeyValues );
	if (!pEntity)
		return NULL;

	AddEntity( pEntity );
	return pEntity;
}

template <class T> T *CEntityManager::AddEntity( const CKeyValues *pKeyValues, CEntityLoadGroup *pLoadGroup )
{
	T *pEntity = CreateEntity<T>( pKeyValues );
	if (!pEntity)
		return NULL;

	AddEntity( pEntity, pLoadGroup );
	return pEntity;
}

template <class T> T *CEntityManager::CreateEntity( const CKeyValues *pKeyValues )
{
	T *pEntity = new T();

	bool bResult = UTIL_LoadKVData( pEntity, pKeyValues );
	if (!bResult)
	{
		delete pEntity;
		return NULL;
	}

	return pEntity;
}

#endif // ENTITYMANAGER_H