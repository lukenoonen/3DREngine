#include "EntityFactory.h"
#include "EntityManager.h"

CBaseEntityFactory::CBaseEntityFactory( const char *sMapName )
{
	m_sMapName = sMapName;

	CEntityManager::AddEntityFactory( this );
}

CBaseEntity *CBaseEntityFactory::CreateEntity( void )
{
	return NULL;
}

const char *CBaseEntityFactory::GetMapName( void )
{
	return m_sMapName;
}