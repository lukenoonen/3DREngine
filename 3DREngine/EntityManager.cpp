#include "EntityManager.h"
#include "GlobalManager.h"
#include "RenderManager.h"

CEntityManager::CEntityManager()
{
	m_pPlayer = NULL;

	m_pShadowCamera = NULL;
	m_pTextureCamera = NULL;
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

			for (unsigned int i = 0; i < (unsigned int)m_pEntities.size(); i++)
			{
				if (m_pEntities[i] == pEntity)
				{
					m_pEntities.erase( m_pEntities.begin() + i );
					break;
				}
			}

			if (pEntity->IsPlayer() && m_pPlayer == pEntity)
			{
				m_pPlayer = NULL;
			}
			else if (pEntity->IsDrawable())
			{
				CBaseDrawable *pDrawable = dynamic_cast<CBaseDrawable *>(pEntity);

				if (pEntity->IsLight())
				{
					for (unsigned int i = 0; i < (unsigned int)m_pLightEntities.size(); i++)
					{
						if (m_pLightEntities[i] == pEntity)
						{
							m_pLightEntities.erase( m_pLightEntities.begin() + i );
							break;
						}
					}
				}
				else if (pEntity->IsCamera())
				{
					for (unsigned int i = 0; i < (unsigned int)m_pCameraEntities.size(); i++)
					{
						if (m_pCameraEntities[i] == pEntity)
						{
							m_pCameraEntities.erase( m_pCameraEntities.begin() + i );
							break;
						}
					}
				}
				else
				{
					for (unsigned int i = 0; i < (unsigned int)m_pDrawEntities.size(); i++)
					{
						if (m_pDrawEntities[i] == pEntity)
						{
							m_pDrawEntities.erase( m_pDrawEntities.begin() + i );
							break;
						}
					}
				}
			}

			delete pEntity;
		}

		m_pEntitiesToRemove.clear();
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

			m_pEntities.push_back( pEntity );

			if (pEntity->IsPlayer() && !m_pPlayer)
			{
				m_pPlayer = dynamic_cast<CBasePlayer *>(pEntity);
			}
			else if (pEntity->IsDrawable())
			{
				if (pEntity->IsLight())
				{
					m_pLightEntities.push_back( dynamic_cast<CBaseLight *>(pEntity) );
				}
				else if (pEntity->IsCamera())
				{
					bool bSuccess = false;
					CBaseCamera *pCamera = dynamic_cast<CBaseCamera *>(pEntity);
					for (unsigned int i = 0; i < (unsigned int)m_pCameraEntities.size(); i++)
					{
						if (pCamera->GetRenderPriority() <= m_pCameraEntities[i]->GetRenderPriority())
						{
							m_pCameraEntities.insert( m_pCameraEntities.begin() + i, pCamera );
							bSuccess = true;
							break;
						}
					}

					if (!bSuccess)
						m_pCameraEntities.push_back( pCamera );
				}
				else
				{
					m_pDrawEntities.push_back( dynamic_cast<CBaseDrawable *>(pEntity) );
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
	pRenderManager->SetRenderPass( ERenderPass::t_depth );
	DrawUnlitEntities();

	pRenderManager->SetRenderPass( ERenderPass::t_unlit );
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

	SetShadowCamera( NULL );
	SetTextureCamera( NULL );
}

void CEntityManager::AddEntity( CBaseEntity *pEntity )
{
	m_pEntitiesToAdd.push_back( pEntity );
}

void CEntityManager::RemoveEntity( CBaseEntity *pEntity )
{
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

CBasePlayer *CEntityManager::GetPlayer( void ) const
{
	return m_pPlayer;
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