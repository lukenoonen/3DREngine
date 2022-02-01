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

	CBasePlayer *GetPlayer( void ) const;

	void SetShadowCamera( CBaseCamera *pShadowCamera ); // TODO: clean up everything relating to this function and...
	void SetTextureCamera( CBaseCamera *pTextureCamera ); // TODO ... this function, as well as their related member variables

	CBaseCamera *GetShadowCamera( void ) const;
	CBaseCamera *GetTextureCamera( void ) const;

private:
	std::vector<CBaseEntity *> m_pEntitiesToAdd;
	std::vector<CBaseEntity *> m_pEntities;
	std::vector<CBaseEntity *> m_pEntitiesToRemove;

	std::vector<CBaseLight *> m_pLightEntities;
	std::vector<CBaseCamera *> m_pCameraEntities;
	std::vector<CBaseDrawable *> m_pDrawEntities;

	CBasePlayer *m_pPlayer;

	CBaseCamera *m_pShadowCamera; // TODO: see if this should be a CBaseShadowCamera or something
	CBaseCamera *m_pTextureCamera; // TODO: figure out a better naming scheme for this, see if this should be a different base class or something
};

#endif // ENTITYMANAGER_H