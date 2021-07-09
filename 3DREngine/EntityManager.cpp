#include "EntityManager.h"
#include "GlobalManager.h"
#include "RenderManager.h"

CEntityManager::CEntityManager()
{
	m_pPlayer = NULL;
}

CEntityManager::~CEntityManager()
{
	Reset();
}

void CEntityManager::Reset( void )
{
	ClearEntities();
}

void CEntityManager::OnLoop( void )
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		if (m_pEntities[i]->IsActive())
			m_pEntities[i]->PreThink();
	}

	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		if (m_pEntities[i]->IsActive())
			m_pEntities[i]->Think();
	}

	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		if (m_pEntities[i]->IsActive())
			m_pEntities[i]->PostThink();
	}
}

void CEntityManager::AddEntity( CBaseEntity *pNewEntity )
{
	if (pNewEntity->IsPlayer() && !m_pPlayer)
		m_pPlayer = dynamic_cast<CPlayer *>(pNewEntity);

	m_pEntities.push_back( pNewEntity );

	pRenderManager->AddEntity( pNewEntity );
}

void CEntityManager::RemoveEntity( CBaseEntity *pEntity )
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		if (m_pEntities[i] == pEntity)
		{
			m_pEntities.erase( m_pEntities.begin() + i );
			break;
		}
	}

	pRenderManager->RemoveEntity( pEntity );
}

void CEntityManager::ClearEntities( void )
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
		delete m_pEntities[i];

	m_pEntities.clear();

	pRenderManager->ClearEntities();
}

CPlayer *CEntityManager::GetPlayer( void )
{
	return m_pPlayer;
}