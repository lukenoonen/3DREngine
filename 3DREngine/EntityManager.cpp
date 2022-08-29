#include "EntityManager.h"
#include "TimeManager.h"
#include "RenderManager.h"

std::vector<CBaseEntityFactory *> *CEntityManager::s_pEntityFactories = NULL;

CEntityManager::CEntityManager()
{
	m_pShadowCamera = NULL;
	m_pTextureCamera = NULL;

	m_pEntityFactories = s_pEntityFactories;
	s_pEntityFactories = NULL;

	m_uiEntityCount = 0;
}

CEntityManager::~CEntityManager()
{
	ClearEntities();

	delete m_pEntityFactories;
}

void CEntityManager::OnLoop( void )
{
	if (m_uiEntityCount != m_pEntities.size())
	{
		for (unsigned int i = m_uiEntityCount; i < m_pEntities.size(); i++)
			m_pEntities[i]->Init(); // What to do when this return false?

		m_uiEntityCount = m_pEntities.size();
	}

	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		CBaseEntity *pEntity = m_pEntities[i];
		if (!pEntity->IsRemoved())
			pEntity->PreThink();
	}

	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		CBaseEntity *pEntity = m_pEntities[i];
		if (!pEntity->IsRemoved())
			pEntity->Think();
	}

	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		CBaseEntity *pEntity = m_pEntities[i];
		if (!pEntity->IsRemoved())
			pEntity->PostThink();
	}

	for (unsigned int i = 0; i < m_pCameraEntities.size(); i++)
	{
		if (m_pCameraEntities[i]->ShouldDraw())
			m_pCameraEntities[i]->Render();
	}

	for (unsigned int i = 0; i < m_pEntities.size(); i++)
	{
		CBaseEntity *pEntity = m_pEntities[i];
		if (pEntity->IsRemoved() && !pEntity->IsReferenced())
		{
			pEntity->Exit();
			RemoveEntity( pEntity );
		}
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
	for (unsigned int i = 0; i < m_pDrawableEntities.size(); i++)
	{
		CBaseDrawable *pDrawable = m_pDrawableEntities[i];
		if (pDrawable->ShouldDraw())
		{
			pRenderManager->SetBlend( false );

			pDrawable->PreDraw();

			// TODO: check to see if this works compared to u iDrawCount
			bool bFirstDraw = true;
			for (unsigned int j = 0; j < m_pLightEntities.size(); j++)
			{
				CBaseLight *pLight = m_pLightEntities[j];
				if (pLight->ShouldDraw())
				{
					pLight->ActivateLight();
					pDrawable->Draw();

					if (bFirstDraw)
					{
						pRenderManager->SetBlend( true );
						bFirstDraw = false;
					}
				}
			}

			pDrawable->PostDraw();
		}
	}

	pRenderManager->SetBlend( false );

	SetTextureCamera( NULL );
	SetShadowCamera( NULL );
}

void CEntityManager::AddEntity( CBaseEntity *pEntity )
{
	m_pEntities.push_back( pEntity );
}

void CEntityManager::RemoveEntity( CBaseEntity *pEntity )
{
	m_pEntities.erase( std::remove( m_pEntities.begin(), m_pEntities.end(), pEntity ) );
	delete pEntity;
}

void CEntityManager::ClearEntities( void ) // TODO: FIX THIS!
{
	for (unsigned int i = 0; i < m_pEntities.size(); i++)
		delete m_pEntities[i];

	m_pEntities.clear();
	m_uiEntityCount = 0;

	m_pPlayerEntities.clear();
	m_pLightEntities.clear();
	m_pCameraEntities.clear();
	m_pDrawableEntities.clear();
}

CBaseEntity *CEntityManager::GetEntityByName( const char *sName, CBaseEntity *pStart )
{
	CBaseEntity *pSearch = pStart ? pStart : m_pRootEntity;
	while (pSearch)
	{
		if (UTIL_StringEquals( sName, pSearch->GetName() ))
			return pSearch;

		pSearch = pSearch->GetNext();
	}

	return NULL;
}

CBaseEntity *CEntityManager::GetEntityByFileName( const char *sFileName, CBaseEntity *pStart )
{
	CBaseEntity *pSearch = pStart ? pStart : m_pRootEntity;
	while (pSearch)
	{
		if (UTIL_StringEquals( sFileName, pSearch->GetFileName() )) // TODO: fix me when completed	
			return pSearch;

		pSearch = pSearch->GetNext();
	}

	return NULL;
}

CBaseEntity *CEntityManager::GetEntityByMapName( const char *sMapName, CBaseEntity *pStart )
{
	CBaseEntity *pSearch = pStart ? pStart : m_pRootEntity;
	while (pSearch)
	{
		if (UTIL_StringEquals( sMapName, pSearch->GetMapName() ))
			return pSearch;

		pSearch = pSearch->GetNext();
	}

	return NULL;
}

void CEntityManager::SetShadowCamera( CBaseCamera *pShadowCamera )
{
	m_pShadowCamera = pShadowCamera;
}

void CEntityManager::SetTextureCamera( CBaseCamera *pTextureCamera )
{
	m_pTextureCamera = pTextureCamera;
}

CBaseCamera *CEntityManager::GetShadowCamera( void ) const
{
	return m_pShadowCamera;
}

CBaseCamera *CEntityManager::GetTextureCamera( void ) const
{
	return m_pTextureCamera;
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
		if (UTIL_StringCompare( (*s_pEntityFactories)[i]->GetMapName(), pEntityFactory->GetMapName() ) <= 0)
		{
			s_pEntityFactories->insert( s_pEntityFactories->begin() + i, pEntityFactory );
			return;
		}
	}

	s_pEntityFactories->push_back( pEntityFactory );
}

CBaseEntity *CEntityManager::CreateEntity( const char *sMapName )
{
	for (unsigned int i = 0; i < m_pEntityFactories->size(); i++)
	{
		CBaseEntityFactory *pEntityFactory = (*m_pEntityFactories)[i];
		if (UTIL_StringEquals( pEntityFactory->GetMapName(), sMapName ))
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