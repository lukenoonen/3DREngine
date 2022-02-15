#include "BaseModeled.h"
#include "GlobalManager.h"
#include "RenderManager.h"
#include "AssetManager.h"

CBaseModeled::CBaseModeled()
{
	for (EBaseEnum i = 0; i < (EBaseEnum)ERenderPass::i_count; i++)
		m_bShouldDraw[i] = false;
}

void CBaseModeled::Exit( void )
{
	for (unsigned int i = 0; i < (unsigned int)m_pGeometry.size(); i++)
	{
		m_pGeometry[i]->Inactivate();
		pAssetManager->CheckGeometry( m_pGeometry[i] );
	}

	for (unsigned int i = 0; i < (unsigned int)m_pMaterials.size(); i++)
	{
		m_pMaterials[i]->Inactivate();
		pAssetManager->CheckMaterial( m_pMaterials[i] );
	}

	BaseClass::Exit();
}

void CBaseModeled::PreDraw( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 0, &GetModelMatrix() );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 1, &GetModelMatrixInverse() );
}

void CBaseModeled::Draw( void )
{
	for (unsigned int i = 0; i < (unsigned int)m_pGeometry.size(); i++)
	{
		if (m_pMaterials[i]->ShouldDraw())
		{
			m_pMaterials[i]->Use();
			m_pGeometry[i]->Draw();
		}
	}
}

bool CBaseModeled::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_bShouldDraw[(EBaseEnum)pRenderManager->GetRenderPass()];
}

void CBaseModeled::AddGeometry( const char *sPath )
{
	CGeometry *pGeometry = pAssetManager->GetGeometry( sPath );
	pGeometry->Activate();
	m_pGeometry.push_back( pGeometry );
}

void CBaseModeled::AddMaterial( const char *sPath )
{
	CMaterial *pMaterial = pAssetManager->GetMaterial( sPath );
	pMaterial->Activate();
	m_pMaterials.push_back( pMaterial );

	ERenderPass eRenderPass = pRenderManager->GetRenderPass();

	for (EBaseEnum i = 0; i < (EBaseEnum)ERenderPass::i_count; i++)
	{
		if (!m_bShouldDraw[i])
		{
			pRenderManager->SetRenderPass( (ERenderPass)i );
			if (pMaterial->ShouldDraw())
				m_bShouldDraw[i] = true;
		}
	}

	pRenderManager->SetRenderPass( eRenderPass );
}