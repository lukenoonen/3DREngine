#include "EntityManager.h"
#include "GlobalManager.h"
#include "RenderManager.h"

CEntityManager::CEntityManager()
{
	m_pPlayer = NULL;
}

CEntityManager::~CEntityManager()
{
	ClearEntities();
}

void CEntityManager::OnLoop( void )
{
	if (!m_pEntitiesToRemove.empty())
	{
		unsigned int uiMinEntityIndex = (unsigned int)m_pEntities.size();
		unsigned int uiMinLightEntityIndex = (unsigned int)m_pLightEntities.size();
		unsigned int uiMinCameraEntityIndex = (unsigned int)m_pCameraEntities.size();
		unsigned int uiMinDrawEntityIndex = (unsigned int)m_pDrawEntities.size();

		for (unsigned int i = 0; i < (unsigned int)m_pEntitiesToRemove.size(); i++)
			m_pEntitiesToRemove[i]->Exit();

		for (unsigned int i = 0; i < (unsigned int)m_pEntitiesToRemove.size(); i++)
		{
			CBaseEntity *pEntity = m_pEntitiesToRemove[i];

			uiMinEntityIndex = pEntity->GetEntityIndex();
			m_pEntities.erase( m_pEntities.begin() + uiMinEntityIndex );

			if (pEntity->IsPlayer() && m_pPlayer == pEntity)
			{
				m_pPlayer = NULL;
			}
			else if (pEntity->IsDrawable())
			{
				CBaseDrawable *pDrawable = dynamic_cast<CBaseDrawable *>(pEntity);

				if (pEntity->IsLight())
				{
					uiMinLightEntityIndex = pDrawable->GetDrawIndex();
					m_pLightEntities.erase( m_pLightEntities.begin() + uiMinLightEntityIndex );
				}
				else if (pEntity->IsCamera())
				{
					uiMinCameraEntityIndex = pDrawable->GetDrawIndex();
					m_pCameraEntities.erase( m_pCameraEntities.begin() + uiMinCameraEntityIndex );
				}
				else
				{
					uiMinDrawEntityIndex = pDrawable->GetDrawIndex();
					m_pDrawEntities.erase( m_pDrawEntities.begin() + uiMinDrawEntityIndex );
				}
			}

			delete pEntity;
		}

		m_pEntitiesToRemove.clear();

		while (uiMinEntityIndex < (unsigned int)m_pEntities.size())
		{
			m_pEntities[uiMinEntityIndex]->SetEntityIndex( uiMinEntityIndex );
			uiMinEntityIndex++;
		}

		while (uiMinEntityIndex < (unsigned int)m_pEntities.size())
		{
			m_pEntities[uiMinEntityIndex]->SetEntityIndex( uiMinEntityIndex );
			uiMinEntityIndex++;
		}

		while (uiMinLightEntityIndex < (unsigned int)m_pCameraEntities.size())
		{
			m_pLightEntities[uiMinLightEntityIndex]->SetEntityIndex( uiMinLightEntityIndex );
			uiMinLightEntityIndex++;
		}

		while (uiMinDrawEntityIndex < (unsigned int)m_pEntities.size())
		{
			m_pDrawEntities[uiMinDrawEntityIndex]->SetEntityIndex( uiMinDrawEntityIndex );
			uiMinDrawEntityIndex++;
		}
	}

	for (unsigned int i = 0; i < (unsigned int)m_pEntities.size(); i++)
	{
		if (m_pEntities[i]->IsActive())
			m_pEntities[i]->PreThink();
	}

	for (unsigned int i = 0; i < (unsigned int)m_pEntities.size(); i++)
	{
		if (m_pEntities[i]->IsActive())
			m_pEntities[i]->Think();
	}

	for (unsigned int i = 0; i < (unsigned int)m_pEntities.size(); i++)
	{
		if (m_pEntities[i]->IsActive())
			m_pEntities[i]->PostThink();
	}

	for (unsigned int i = 0; i < (unsigned int)m_pCameraEntities.size(); i++)
	{
		if (m_pCameraEntities[i]->ShouldDraw())
			m_pCameraEntities[i]->Render();
	}

	if (!m_pEntitiesToAdd.empty())
	{
		for (unsigned int i = 0; i < (unsigned int)m_pEntitiesToAdd.size(); i++)
		{
			CBaseEntity *pEntity = m_pEntitiesToAdd[i];

			pEntity->SetEntityIndex( (unsigned int)m_pEntities.size() );
			m_pEntities.push_back( pEntity );

			if (pEntity->IsPlayer() && !m_pPlayer)
			{
				m_pPlayer = dynamic_cast<CBasePlayer *>(pEntity);
			}
			else if (pEntity->IsDrawable())
			{
				if (pEntity->IsLight())
				{
					CBaseLight *pLight = dynamic_cast<CBaseLight *>(pEntity);
					pLight->SetDrawIndex( (unsigned int)m_pLightEntities.size() );
					m_pLightEntities.push_back( pLight );
				}
				else if (pEntity->IsCamera())
				{
					bool bSuccess = false;
					CBaseCamera *pCamera = dynamic_cast<CBaseCamera *>(pEntity);
					for (unsigned int i = 0; i < (unsigned int)m_pCameraEntities.size(); i++)
					{
						if (pCamera->GetRenderPriority() <= m_pCameraEntities[i]->GetRenderPriority())
						{
							pCamera->SetDrawIndex( i );
							m_pCameraEntities.insert( m_pCameraEntities.begin() + i, pCamera );
							bSuccess = true;
							break;
						}
					}

					if (!bSuccess)
					{
						pCamera->SetDrawIndex( (unsigned int)m_pCameraEntities.size() );
						m_pCameraEntities.push_back( pCamera );
					}
				}
				else
				{
					CBaseDrawable *pDrawable = dynamic_cast<CBaseDrawable *>(pEntity);
					pDrawable->SetDrawIndex( (unsigned int)m_pDrawEntities.size() );
					m_pDrawEntities.push_back( pDrawable );
				}
			}
		}

		for (unsigned int i = 0; i < (unsigned int)m_pEntitiesToAdd.size(); i++)
			m_pEntitiesToAdd[i]->Init();

		m_pEntitiesToAdd.clear();
	}
}

void CEntityManager::DrawEntities( void )
{
	pRenderManager->SetRenderPass( RENDERPASS_DEPTH );
	DrawUnlitEntities();

	pRenderManager->SetRenderPass( RENDERPASS_UNLIT );
	DrawUnlitEntities();

	DrawLitEntities();
}

void CEntityManager::DrawUnlitEntities( void )
{
	for (unsigned int i = 0; i < m_pDrawEntities.size(); i++)
	{
		if (m_pDrawEntities[i]->ShouldDraw())
		{
			m_pDrawEntities[i]->PreDraw();
			m_pDrawEntities[i]->Draw();
			m_pDrawEntities[i]->PostDraw();
		}
	}
}

void CEntityManager::DrawLitEntities( void )
{
	for (unsigned int i = 0; i < m_pDrawEntities.size(); i++)
	{
		if (m_pDrawEntities[i]->ShouldDraw())
		{
			pRenderManager->SetBlend( false );
			m_pDrawEntities[i]->PreDraw();
			unsigned int uiDrawCount = 0;
			for (unsigned int j = 0; j < m_pLightEntities.size(); j++)
			{
				if (m_pLightEntities[j]->ShouldDraw())
				{
					if (uiDrawCount == 1)
						pRenderManager->SetBlend( true );

					m_pLightEntities[j]->ActivateLight();
					m_pDrawEntities[i]->Draw();
					uiDrawCount++;
				}
			}
			m_pDrawEntities[i]->PostDraw();
		}
	}

	pRenderManager->SetBlend( false );
}

void CEntityManager::AddEntity( CBaseEntity *pEntity )
{
	m_pEntitiesToAdd.push_back( pEntity );
}

void CEntityManager::RemoveEntity( CBaseEntity *pEntity )
{
	m_pEntitiesToRemove.push_back( pEntity );

	bool bSuccess = false;
	for (unsigned int i = 0; i < (unsigned int)m_pEntitiesToRemove.size(); i++)
	{
		if (pEntity->GetEntityIndex() > m_pEntitiesToRemove[i]->GetEntityIndex())
		{
			m_pEntitiesToRemove.insert( m_pEntitiesToRemove.begin() + i, pEntity );
			bSuccess = true;
			break;
		}
	}

	if (!bSuccess)
		m_pEntitiesToRemove.push_back( pEntity );
}

void CEntityManager::ClearEntities( void )
{
	for (unsigned int i = 0; i < (unsigned int)m_pEntitiesToAdd.size(); i++)
		delete m_pEntitiesToAdd[i];

	for (unsigned int i = 0; i < (unsigned int)m_pEntities.size(); i++)
		delete m_pEntities[i];

	m_pEntitiesToAdd.clear();
	m_pEntities.clear();
	m_pEntitiesToRemove.clear();

	m_pLightEntities.clear();
	m_pCameraEntities.clear();
	m_pDrawEntities.clear();

	m_pPlayer = NULL;
}

CBasePlayer *CEntityManager::GetPlayer( void )
{
	return m_pPlayer;
}