#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include "Global.h"
#include "BaseEntity.h"
#include "BasePlayer.h"
#include "BaseDrawable.h"
#include "BaseLight.h"
#include "BaseCamera.h"
#include "EntityFactory.h"

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

	void SetShadowCamera( CBaseCamera *pShadowCamera ); // TODO: clean up everything relating to this function and...
	void SetTextureCamera( CBaseCamera *pTextureCamera ); // TODO ... this function, as well as their related member variables

	CBaseCamera *GetShadowCamera( void ) const;
	CBaseCamera *GetTextureCamera( void ) const;

	// TODO: figure out a better solution to this
	CBasePlayer *GetPlayer( unsigned int uiIndex );

	static void AddEntityFactory( CBaseEntityFactory *pEntityFactory ); // TODO: Consider cleaning this up

	bool AddEntityTest( const char *sMapName, const CTextBlock *pTextBlock );

private:
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

	CBaseCamera *m_pShadowCamera; // TODO: see if this should be a CBaseShadowCamera or something
	CBaseCamera *m_pTextureCamera; // TODO: figure out a better naming scheme for this, see if this should be a different base class or something

	static std::vector<CBaseEntityFactory *> *s_pEntityFactories;
	std::vector<CBaseEntityFactory *> *m_pEntityFactories;
};

#endif // ENTITYMANAGER_H