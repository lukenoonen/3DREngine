#include "BaseModeled.h"
#include "GlobalManager.h"
#include "RenderManager.h"
#include "AssetManager.h"

CBaseModeled::CBaseModeled()
{
	m_pModel = NULL;
}

void CBaseModeled::PreDraw( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 0, &GetModelMatrix() );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 1, &GetModelMatrixInverse() );
}

void CBaseModeled::Draw( void )
{
	m_pModel->Draw();
}

void CBaseModeled::SetModel( CModel *pModel )
{
	m_pModel = pModel;
}

CModel *CBaseModeled::GetModel( void ) const
{
	return m_pModel;
}

bool CBaseModeled::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_pModel != NULL && m_pModel->ShouldDraw();
}