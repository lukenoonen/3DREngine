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

	return pEntityManager->AddEntityTest( sMapName, pTextBlock );
}
CConCommand cc_createentity( "createentity", CC_CreateEntity );

std::vector<CBaseEntityFactory *> *CEntityManager::s_pEntityFactories = NULL;

std::vector<CEntityFlag *> *CEntityManager::s_pEntityFlags = NULL;

CEntityManager::CEntityManager()
{
	m_uiEntityCount = 0;

	m_pCurrentCamera = NULL;
	m_pCurrentLight = NULL;

	m_pEntityFactories = s_pEntityFactories;
	s_pEntityFactories = NULL;

	m_pEntityFlags = s_pEntityFlags;
	s_pEntityFlags = NULL;
}

CEntityManager::~CEntityManager()
{
	ClearEntities();

	delete m_pEntityFactories;

	delete m_pEntityFlags;
}

void CEntityManager::OnLoop( void )
{
	// TODO: formalise the way newly-added entities are handled
	if (m_uiEntityCount != m_pEntities.size())
	{
		for (unsigned int i = m_uiEntityCount; i < m_pEntities.size(); i++)
		{
			CBaseEntity *pEntity = m_pEntities[i];

			if (pEntity->IsCamera())
				AddCamera( (CBaseCamera *)pEntity );
			if (pEntity->IsDrawable())
				AddDrawable( (CBaseDrawable *)pEntity );
			if (pEntity->IsLight())
				AddLight( (CBaseLight *)pEntity );
			if (pEntity->IsPlayer())
				AddPlayer( (CBasePlayer *)pEntity );
		}

		for (unsigned int i = m_uiEntityCount; i < m_pEntities.size(); i++)
			UTIL_LinkData( m_pEntities[i] );

		for (unsigned int i = m_uiEntityCount; i < m_pEntities.size(); i++)
			m_pEntities[i]->Init();

		m_uiEntityCount = (unsigned int)m_pEntities.size();
	}

	for (unsigned int i = 0; i < m_uiEntityCount; i++)
		m_pEntities[i]->PreThink();

	for (unsigned int i = 0; i < m_uiEntityCount; i++)
		m_pEntities[i]->Think();

	for (unsigned int i = 0; i < m_uiEntityCount; i++)
		m_pEntities[i]->PostThink();

	for (unsigned int i = 0; i < m_pCameraEntities.size(); i++)
	{
		m_pCurrentCamera = m_pCameraEntities[i];
		if (m_pCurrentCamera->ShouldDraw())
			m_pCurrentCamera->Render();
	}

	for (unsigned int i = 0; i < m_pEntitiesToRemove.size(); i++)
	{
		CBaseEntity *pEntityToRemove = m_pEntitiesToRemove[i];
		if (!pEntityToRemove->IsReferenced())
			delete pEntityToRemove;
	}
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

void CEntityManager::DrawLitEntities( void )
{
	pRenderManager->SetBlend( false );

	for (unsigned int i = 0; i < m_pLightEntities.size(); i++)
	{
		m_pCurrentLight = m_pLightEntities[i];

		if (m_pCurrentLight->ShouldDraw())
		{
			m_pCurrentLight->ActivateLight();
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

			pRenderManager->SetBlend( true );
		}
	}

	m_pCurrentLight = NULL;

	pRenderManager->SetBlend( false );
}

void CEntityManager::AddEntity( CBaseEntity *pEntity )
{
	m_pEntities.push_back( pEntity );
}

void CEntityManager::RemoveEntity( CBaseEntity *pEntity )
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		if (m_pEntities[i] == pEntity)
		{
			m_pEntities.erase( m_pEntities.begin() + i );
			m_uiEntityCount--;

			m_pEntitiesToRemove.push_back( pEntity );

			if (pEntity->IsCamera())
			{
				for (unsigned int j = 0; j < m_pCameraEntities.size(); j++)
				{
					if (m_pCameraEntities[j] == pEntity)
						m_pCameraEntities.erase( m_pCameraEntities.begin() + j );
				}
			}
			if (pEntity->IsDrawable())
			{
				for (unsigned int j = 0; j < m_pDrawableEntities.size(); j++)
				{
					if (m_pDrawableEntities[j] == pEntity)
						m_pDrawableEntities.erase( m_pDrawableEntities.begin() + j );
				}
			}
			if (pEntity->IsLight())
			{
				for (unsigned int j = 0; j < m_pLightEntities.size(); j++)
				{
					if (m_pLightEntities[j] == pEntity)
						m_pLightEntities.erase( m_pLightEntities.begin() + j );
				}
			}
			if (pEntity->IsPlayer())
			{
				for (unsigned int j = 0; j < m_pPlayerEntities.size(); j++)
				{
					if (m_pPlayerEntities[j] == pEntity)
						m_pPlayerEntities.erase( m_pPlayerEntities.begin() + j );
				}
			}

			pEntity->Remove();
		}
	}
}

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
}

CBaseEntity *CEntityManager::GetEntityByName( const char *sName ) // TODO: figure out a good way to search through only the current load, like GetEntityByIndex
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		CBaseEntity *pEntity = m_pEntities[i];
		if (UTIL_streq( pEntity->GetName(), sName ))
			return pEntity;
	}

	return NULL;
}

CBaseEntity *CEntityManager::GetEntityByFileName( const char *sFileName )
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		CBaseEntity *pEntity = m_pEntities[i];
		if (UTIL_streq( pEntity->GetFileName(), sFileName ))
			return pEntity;
	}

	return NULL;
}

CBaseEntity *CEntityManager::GetEntityByMapName( const char *sMapName )
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		CBaseEntity *pEntity = m_pEntities[i];
		if (UTIL_streq( pEntity->GetMapName(), sMapName ))
			return pEntity;
	}

	return NULL;
}

CBaseEntity *CEntityManager::GetEntityByIndex( unsigned int uiIndex )
{
	if (uiIndex == 0)
		return NULL;

	uiIndex += m_uiEntityCount - 1;
	if (m_pEntities.size() >= uiIndex)
		return NULL;

	return m_pEntities[uiIndex];
}

unsigned int CEntityManager::GetEntityIndex( CBaseEntity *pEntity )
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		if (m_pEntities[i] == pEntity)
			return i + 1;
	}

	return 0;
}

CBaseCamera *CEntityManager::GetCurrentCamera( void ) const
{
	return m_pCurrentCamera;
}

CBaseLight *CEntityManager::GetCurrentLight( void ) const
{
	return m_pCurrentLight;
}

CBasePlayer *CEntityManager::GetPlayer( unsigned int uiIndex )
{
	return m_pPlayerEntities[uiIndex];
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

bool CEntityManager::AddEntityTest( const char *sMapName, const CTextBlock *pTextBlock )
{
	CBaseEntity *pEntity = CreateEntity( sMapName );
	if (!pEntity)
		return false;

	bool bResult = UTIL_LoadTextData( pEntity, pTextBlock );
	if (!bResult)
	{
		delete pEntity;
		return false;
	}

	AddEntity( pEntity );
	return true;
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

#include <iostream>

void CEntityManager::AddCamera( CBaseCamera *pCamera )
{
	std::cout << pCamera->GetMapName() << '\n';
	int iCameraPriority = pCamera->GetPriority();
	for (unsigned int i = 0; i < m_pCameraEntities.size(); i++)
	{
		if (iCameraPriority <= m_pCameraEntities[i]->GetPriority())
		{
			m_pCameraEntities.insert( m_pCameraEntities.begin() + i, pCamera );
			std::cout << i << '\n';
			return;
		}
	}

	m_pCameraEntities.push_back( pCamera );
	std::cout << m_pCameraEntities.size() << '\n';
}

void CEntityManager::AddDrawable( CBaseDrawable *pDrawable )
{
	m_pDrawableEntities.push_back( pDrawable );
}

void CEntityManager::AddLight( CBaseLight *pLight )
{
	m_pLightEntities.push_back( pLight );
}

void CEntityManager::AddPlayer( CBasePlayer *pPlayer )
{
	m_pPlayerEntities.push_back( pPlayer );
}

CBaseEntity *CEntityManager::CreateEntity( const char *sMapName )
{
	for (unsigned int i = 0; i < m_pEntityFactories->size(); i++)
	{
		CBaseEntityFactory *pEntityFactory = (*m_pEntityFactories)[i];
		if (UTIL_streq( pEntityFactory->GetMapName(), sMapName ))
			return pEntityFactory->CreateEntity();
	}

	return NULL;
}

CBaseEntity *CEntityManager::CreateEntity( unsigned int uiEntityIndex )
{
	if (uiEntityIndex >= m_pEntityFactories->size())
		return NULL;

	return (*m_pEntityFactories)[uiEntityIndex]->CreateEntity();
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