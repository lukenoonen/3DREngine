#include "EntityManager.h"
#include "TimeManager.h"
#include "RenderManager.h"
#include "FileManager.h"

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

CEntityManager::CEntityManager()
{
	m_uiEntityCount = 0;

	m_pShadowCamera = NULL;
	m_pTextureCamera = NULL;

	m_pEntityFactories = s_pEntityFactories;
	s_pEntityFactories = NULL;
}

CEntityManager::~CEntityManager()
{
	ClearEntities();

	delete m_pEntityFactories;
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
				m_pCameraEntities.push_back( (CBaseCamera *)pEntity );
			if (pEntity->IsDrawable())
				m_pDrawableEntities.push_back( (CBaseDrawable *)pEntity );
			if (pEntity->IsLight())
				m_pLightEntities.push_back( (CBaseLight *)pEntity );
			if (pEntity->IsPlayer())
				m_pPlayerEntities.push_back( (CBasePlayer *)pEntity );
		}

		for (unsigned int i = m_uiEntityCount; i < m_pEntities.size(); i++)
		{
			UTIL_LinkData( m_pEntities[i] );
		}

		for (unsigned int i = m_uiEntityCount; i < m_pEntities.size(); i++)
		{
			m_pEntities[i]->Init();
		}

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
		if (m_pCameraEntities[i]->ShouldDraw())
			m_pCameraEntities[i]->Render();
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
	//pRenderManager->SetRenderPass( ERenderPass::t_depth );
	//DrawUnlitEntities();

	glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );

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

	bool bFirstDraw = true;

	for (unsigned int i = 0; i < m_pLightEntities.size(); i++)
	{
		CBaseLight *pLight = m_pLightEntities[i];
		if (pLight->ShouldDraw())
		{
			pLight->ActivateLight();
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

	pRenderManager->SetBlend( false );

	/*

	for (unsigned int i = 0; i < m_pDrawableEntities.size(); i++)
	{
		CBaseDrawable *pDrawable = m_pDrawableEntities[i];
		if (pDrawable->ShouldDraw())
		{
			pRenderManager->SetBlend( false );

			pDrawable->PreDraw();

			// TODO: check to see if this works compared to uiDrawCount
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

	pRenderManager->SetBlend( false );*/
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

	bool bResult = UTIL_LoadTextData( pEntity, pTextBlock ); // && UTIL_LinkData( pEntity ); // Figure out the right place to put this
	if (!bResult)
	{
		delete pEntity;
		return false;
	}

	AddEntity( pEntity );
	return true;
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