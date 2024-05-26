#include "EntityManager.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "FileManager.h"
#include "BaseEntity.h"
#include "BasePlayer.h"
#include "BaseDrawable.h"
#include "BaseLight.h"
#include "BaseCamera.h"

bool CC_CreateEntity( const CTextLine *pCommand )
{
	const char *sMapName;
	if (!pCommand->GetValue( sMapName, 1 ))
		return false;

	CTextBlock *pTextBlock;
	if (!pCommand->GetValue( pTextBlock, 2 ))
		return false;

	return pEntityManager->AddEntity( sMapName, pTextBlock );
}
CConCommand cc_createentity( "createentity", CC_CreateEntity );

CEntityLoadGroup::CEntityLoadGroup()
{
	m_uiEntityCount = 0;
}

CEntityLoadGroup::~CEntityLoadGroup()
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
		delete m_pEntities[i];

	for (unsigned int i = 0; i < m_pEntitiesToRemove.size(); i++)
		delete m_pEntitiesToRemove[i];
}

void CEntityLoadGroup::ProcessAddedEntities( void )
{

}

void CEntityLoadGroup::PreThink( void )
{
	for (unsigned int i = 0; i < m_uiEntityCount; i++)
		m_pEntities[i]->PreThink();
}

void CEntityLoadGroup::Think( void )
{
	for (unsigned int i = 0; i < m_uiEntityCount; i++)
		m_pEntities[i]->Think();
}

void CEntityLoadGroup::PostThink( void )
{
	for (unsigned int i = 0; i < m_uiEntityCount; i++)
		m_pEntities[i]->PostThink();
}

void CEntityLoadGroup::PreRender( void )
{
	for (unsigned int i = 0; i < m_pDrawableEntities.size(); i++)
		m_pDrawableEntities[i]->PreRender();
}

void CEntityLoadGroup::Render( void )
{
	for (unsigned int i = 0; i < m_pCameraEntities.size(); i++)
	{
		CBaseCamera *pCamera = m_pCameraEntities[i];
		pEntityManager->SetCurrentCamera( pCamera );
		if (pCamera->ShouldDraw())
			pCamera->Render();
	}
}

void CEntityLoadGroup::DrawUnlitEntities( void )
{
	for (unsigned int i = 0; i < m_pDrawableEntities.size(); i++)
	{
		CBaseDrawable *pDrawable = m_pDrawableEntities[i];
		if (pDrawable->ShouldDraw())
		{
			pDrawable->PreDraw();
			pDrawable->Draw();
			pDrawable->PostDraw();
		}
	}
}

void CEntityLoadGroup::DrawLitEntities( void )
{
	for (unsigned int i = 0; i < m_pLightEntities.size(); i++)
	{
		CBaseLight *pLight = m_pLightEntities[i];
		pEntityManager->SetCurrentLight( pLight );
		if (pLight->ShouldDraw())
		{
			pLight->ActivateLight();
			pEntityManager->DrawUnlitEntities();
		}
	}
}

#include <iostream>

void CEntityLoadGroup::ProcessRemovedEntities( void )
{
	for (unsigned int i = 0; i < m_pEntitiesToRemove.size(); i++)
	{
		CBaseEntity *pEntityToRemove = m_pEntitiesToRemove[i];
		std::cout << (pEntityToRemove->GetName() ? pEntityToRemove->GetName() : "NULL") << "\n";
		pEntityToRemove->OnRemove(); // TODO: KEEP CLEANING!! TEMPORARY BTW
		if (!pEntityToRemove->IsReferenced())
		{
			m_pEntitiesToRemove.erase( m_pEntitiesToRemove.begin() + i );
			delete pEntityToRemove;
		}
	}
}

CBaseEntity *CEntityLoadGroup::GetEntityByName( const char *sName ) const
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		CBaseEntity *pEntity = m_pEntities[i];
		const char *sEntityName = pEntity->GetName();
		if (sEntityName && UTIL_streq( sEntityName, sName ))
			return pEntity;
	}

	return NULL;
}

CBaseEntity *CEntityLoadGroup::GetEntityByIndex( unsigned int uiIndex ) const
{
	if (m_pEntities.size() >= uiIndex)
		return NULL;

	return m_pEntities[uiIndex];
}

unsigned int CEntityLoadGroup::GetEntityIndex( const CBaseEntity *pEntity ) const
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		if (m_pEntities[i] == pEntity)
			return i;
	}

	return INVALID_INDEX;
}

void CEntityLoadGroup::AddEntity( CBaseEntity *pEntity )
{
	m_pEntities.push_back( pEntity );
	pEntity->SetLoadGroup( this );
}

void CEntityLoadGroup::Flush( void )
{
	// TODO: what happens if UTIL_LinkData returns false?
	for (unsigned int i = m_uiEntityCount; i < m_pEntities.size(); i++)
		UTIL_LinkData( m_pEntities[i] );

	for (unsigned int i = m_uiEntityCount; i < m_pEntities.size(); i++)
		m_pEntities[i]->Init();

	for (unsigned int i = m_uiEntityCount; i < m_pEntities.size(); i++)
	{
		CBaseEntity *pEntity = m_pEntities[i];

		if (pEntity->IsCamera())
			AddCamera( (CBaseCamera *)pEntity );
		if (pEntity->IsDrawable())
			AddDrawable( (CBaseDrawable *)pEntity );
		if (pEntity->IsLight())
			AddLight( (CBaseLight *)pEntity );
	}

	m_uiEntityCount = (unsigned int)m_pEntities.size();
}

void CEntityLoadGroup::RemoveEntity( CBaseEntity *pEntity )
{
	if (RemoveEntityImmediate( pEntity ))
		m_pEntitiesToRemove.push_back( pEntity );
}

void CEntityLoadGroup::ChangeLoadGroup( CBaseEntity *pEntity )
{
	CEntityLoadGroup *pPrevLoadGroup = pEntity->GetLoadGroup();
	if (pPrevLoadGroup)
		pPrevLoadGroup->RemoveEntityImmediate( pEntity );

	m_pEntities.insert( m_pEntities.begin() + m_uiEntityCount, pEntity );
	m_uiEntityCount++;
}

bool CEntityLoadGroup::RemoveEntityImmediate( CBaseEntity *pEntity )
{
	unsigned int uiEntityIndex = GetEntityIndex( pEntity );
	if (uiEntityIndex == INVALID_INDEX)
		return false;

	m_pEntities.erase( m_pEntities.begin() + uiEntityIndex );
	m_uiEntityCount--;

	if (pEntity->IsCamera())
		m_pCameraEntities.erase( std::find( m_pCameraEntities.begin(), m_pCameraEntities.end(), (CBaseCamera *)pEntity ) );
	if (pEntity->IsDrawable())
		m_pDrawableEntities.erase( std::find( m_pDrawableEntities.begin(), m_pDrawableEntities.end(), (CBaseDrawable *)pEntity ) );
	if (pEntity->IsLight())
		m_pLightEntities.erase( std::find( m_pLightEntities.begin(), m_pLightEntities.end(), (CBaseLight *)pEntity ) );

	return true;
}

void CEntityLoadGroup::AddCamera( CBaseCamera *pCamera )
{
	int iCameraPriority = pCamera->GetPriority();
	for (unsigned int i = 0; i < m_pCameraEntities.size(); i++)
	{
		if (iCameraPriority <= m_pCameraEntities[i]->GetPriority())
		{
			m_pCameraEntities.insert( m_pCameraEntities.begin() + i, pCamera );
			return;
		}
	}

	m_pCameraEntities.push_back( pCamera );
}

void CEntityLoadGroup::AddDrawable( CBaseDrawable *pDrawable )
{
	m_pDrawableEntities.push_back( pDrawable );
}

void CEntityLoadGroup::AddLight( CBaseLight *pLight )
{
	m_pLightEntities.push_back( pLight );
}

std::vector<CBaseEntityFactory *> *CEntityManager::s_pEntityFactories = NULL;

std::vector<CEntityFlag *> *CEntityManager::s_pEntityFlags = NULL;

CEntityManager::CEntityManager()
{

	m_LoadGroups.emplace_back();
	m_pGlobalLoadGroup = &m_LoadGroups.back();
	m_pActiveLoadGroup = NULL;

	m_pLocalPlayer = NULL;
	m_pCurrentCamera = NULL;
	m_pCurrentLight = NULL;

	m_pEntityFactories = s_pEntityFactories;
	s_pEntityFactories = NULL;

	m_pEntityFlags = s_pEntityFlags;
	s_pEntityFlags = NULL;
}

CEntityManager::~CEntityManager()
{
	delete m_pEntityFactories;
	delete m_pEntityFlags;
}

void CEntityManager::OnLoop( void )
{
	FlushLoadGroup();

	for (unsigned int i = 0; i < m_LoadGroups.size(); i++)
		m_LoadGroups[i].ProcessAddedEntities();

	for (unsigned int i = 0; i < m_LoadGroups.size(); i++)
		m_LoadGroups[i].PreThink();

	for (unsigned int i = 0; i < m_LoadGroups.size(); i++)
		m_LoadGroups[i].Think();

	for (unsigned int i = 0; i < m_LoadGroups.size(); i++)
		m_LoadGroups[i].PostThink();

	for (unsigned int i = 0; i < m_LoadGroups.size(); i++)
		m_LoadGroups[i].PreRender();

	for (unsigned int i = 0; i < m_LoadGroups.size(); i++)
		m_LoadGroups[i].Render();

	for (unsigned int i = 0; i < m_LoadGroups.size(); i++)
		m_LoadGroups[i].ProcessRemovedEntities();
}

void CEntityManager::DrawEntities( void )
{
	pRenderManager->SetRenderPass( ERenderPass::t_depth );
	DrawUnlitEntities();

	pRenderManager->SetRenderPass( ERenderPass::t_unlit );
	DrawUnlitEntities();

	DrawLitEntities();
}

void CEntityManager::DrawUnlitEntities( void )
{
	for (unsigned int i = 0; i < m_LoadGroups.size(); i++)
		m_LoadGroups[i].DrawUnlitEntities();
}

void CEntityManager::DrawLitEntities( void )
{
	for (unsigned int i = 0; i < m_LoadGroups.size(); i++)
		m_LoadGroups[i].DrawLitEntities();

	m_pCurrentLight = NULL;
}

void CEntityManager::CreateLoadGroup( void )
{
	if (m_pActiveLoadGroup)
		m_pActiveLoadGroup->Flush();

	m_LoadGroups.emplace_back();
	m_pActiveLoadGroup = &m_LoadGroups.back();
}

void CEntityManager::FlushLoadGroup( void )
{
	if (m_pActiveLoadGroup)
	{
		m_pActiveLoadGroup->Flush();
		m_pActiveLoadGroup = NULL;
	}
}

void CEntityManager::FlushLoadGroup( CEntityLoadGroup *pLoadGroup )
{
	if (!pLoadGroup)
		pLoadGroup = m_pGlobalLoadGroup;

	pLoadGroup->Flush();
}

CBaseEntity *CEntityManager::AddEntity( const char *sMapName, const CTextBlock *pTextBlock )
{
	CBaseEntity *pEntity = CreateEntity( sMapName, pTextBlock );
	if (!pEntity)
		return NULL;

	AddEntity( pEntity );
	return pEntity;
}

CBaseEntity *CEntityManager::AddEntity( const char *sMapName, const CTextBlock *pTextBlock, CEntityLoadGroup *pLoadGroup )
{
	CBaseEntity *pEntity = CreateEntity( sMapName, pTextBlock );
	if (!pEntity)
		return NULL;

	AddEntity( pEntity, pLoadGroup );
	return pEntity;
}

/*
void CEntityManager::ClearEntities( void ) // TODO: FIX THIS!
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
		delete m_pEntities[i];

	for (unsigned int i = 0; i < m_pEntitiesToRemove.size(); i++)
		delete m_pEntitiesToRemove[i];

	m_pEntities.clear();
	m_uiEntityCount = 0;

	m_pEntitiesToRemove.clear();

	m_pPlayerEntities.clear();
	m_pLightEntities.clear();
	m_pCameraEntities.clear();
	m_pDrawableEntities.clear();
}*/

CBaseEntity *CEntityManager::GetEntityByName( const char *sName ) const
{
	if (!m_pActiveLoadGroup)
		return NULL;

	return m_pActiveLoadGroup->GetEntityByName( sName );
}

CBaseEntity *CEntityManager::GetEntityByIndex( unsigned int uiIndex ) const
{
	if (!m_pActiveLoadGroup)
		return NULL;

	return m_pActiveLoadGroup->GetEntityByIndex( uiIndex );
}

void CEntityManager::SetLocalPlayer( CBasePlayer *pPlayer )
{
	m_pLocalPlayer = pPlayer;
}

CBasePlayer *CEntityManager::GetLocalPlayer( void ) const
{
	return m_pLocalPlayer;
}

void CEntityManager::SetCurrentCamera( CBaseCamera *pCamera )
{
	m_pCurrentCamera = pCamera;
}

CBaseCamera *CEntityManager::GetCurrentCamera( void ) const
{
	return m_pCurrentCamera;
}

void CEntityManager::SetCurrentLight( CBaseLight *pLight )
{
	m_pCurrentLight = pLight;
}

CBaseLight *CEntityManager::GetCurrentLight( void ) const
{
	return m_pCurrentLight;
}

void CEntityManager::AddEntityFactory( CBaseEntityFactory *pEntityFactory )
{
	if (!s_pEntityFactories)
		s_pEntityFactories = new std::vector<CBaseEntityFactory *>();

	for (unsigned int i = 0; i < s_pEntityFactories->size(); i++)
	{
		if (UTIL_strcmp( (*s_pEntityFactories)[i]->GetMapName(), pEntityFactory->GetMapName() ) <= 0)
		{
			s_pEntityFactories->insert( s_pEntityFactories->begin() + i, pEntityFactory );
			return;
		}
	}

	s_pEntityFactories->push_back( pEntityFactory );
}

void CEntityManager::AddFlag( CEntityFlag *pEntityFlag )
{
	if (!s_pEntityFlags)
		s_pEntityFlags = new std::vector<CEntityFlag *>();

	s_pEntityFlags->push_back( pEntityFlag );
}

int CEntityManager::GetFlag( const char *sKey ) const
{
	for (unsigned int i = 0; i < m_pEntityFlags->size(); i++)
	{
		CEntityFlag *pEntityFlag = m_pEntityFlags->at( i );
		if (UTIL_streq( sKey, pEntityFlag->GetKey() ))
			return pEntityFlag->GetFlag();
	}

	return -1;
}

void CEntityManager::AddEntity( CBaseEntity *pEntity )
{
	if (!m_pActiveLoadGroup)
		m_pActiveLoadGroup = m_pGlobalLoadGroup;

	m_pActiveLoadGroup->AddEntity( pEntity );
}

void CEntityManager::AddEntity( CBaseEntity *pEntity, CEntityLoadGroup *pLoadGroup )
{
	if (!pLoadGroup)
		pLoadGroup = m_pGlobalLoadGroup;

	pLoadGroup->AddEntity( pEntity );
}

CBaseEntity *CEntityManager::CreateEntity( unsigned int uiEntityIndex )
{
	if (uiEntityIndex >= m_pEntityFactories->size())
		return NULL;

	return (*m_pEntityFactories)[uiEntityIndex]->CreateEntity();
}

CBaseEntity *CEntityManager::CreateEntity( const char *sMapName, const CTextBlock *pTextBlock )
{
	CBaseEntity *pEntity = NULL;
	for (unsigned int i = 0; i < m_pEntityFactories->size(); i++)
	{
		CBaseEntityFactory *pEntityFactory = (*m_pEntityFactories)[i];
		if (UTIL_streq( pEntityFactory->GetMapName(), sMapName ))
		{
			pEntity = pEntityFactory->CreateEntity();
			break;
		}
	}

	if (!pEntity)
		return NULL;

	bool bResult = UTIL_LoadTextData( pEntity, pTextBlock );
	if (!bResult)
	{
		delete pEntity;
		return NULL;
	}

	return pEntity;
}

CBaseEntity *CEntityManager::LoadEntity( const char *sFileName )
{
	if (!pFileManager->Open( sFileName, EFileType::t_entity ))
		return NULL;

	unsigned int uiEntityIndex;
	if (!pFileManager->Read( uiEntityIndex ))
	{
		pFileManager->Close();
		return NULL;
	}

	CBaseEntity *pEntity = CreateEntity( uiEntityIndex );
	if (!pEntity)
		return NULL;

	bool bResult = UTIL_LoadData( pEntity );
	pFileManager->Close();

	if (!bResult)
	{
		delete pEntity;
		return NULL;
	}

	return pEntity;
}

/*
	CBaseEntity *pEntity = NULL;

	if (bText)
	{
		char *sBuffer;
		if (!pFileManager->Buffer( sFileName, EFileType::t_entitytext, sBuffer ))
			return false;

		CTextReader trTextReader( sBuffer );
		CTextBlock *pTextBlock = trTextReader.GetTextBlock();
		delete[] sBuffer;

		const char *sMapName;
		if (!pTextBlock->GetValue( "mapname", sMapName ))
			return false;

		pEntity = CreateEntity( sMapName );

		CDataMap *pDataMap = pEntity->GetDataMap();
		while (pDataMap)
		{
			if (!pDataMap->Load( pEntity, pTextBlock ))
			{
				delete pEntity;
				return NULL;
			}

			pDataMap = pDataMap->GetBaseMap();
		}
	}
	else
	{
		if (!pFileManager->Open( sFileName, EFileType::t_entity ))
			return NULL;

		unsigned short usEntityIndex;
		if (!pFileManager->Read( usEntityIndex ))
		{
			pFileManager->Close();
			return NULL;
		}

		pEntity = CreateEntity( usEntityIndex );

		CDataMap *pDataMap = pEntity->GetDataMap();
		while (pDataMap)
		{
			if (!pDataMap->Load( pEntity ))
			{
				delete pEntity;
				return NULL;
			}

			pDataMap = pDataMap->GetBaseMap();
		}
	}

	return pEntity;*/