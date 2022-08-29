#include "BaseModeled.h"
#include "TimeManager.h"
#include "RenderManager.h"

CBaseModeled::CBaseModeled()
{
	m_pModel = NULL;
}

void CBaseModeled::PreDraw( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 0, &GetModelMatrix() ); // TODO: change "model" matrix to better name
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 1, &GetModelMatrixInverse() );
}

void CBaseModeled::Draw( void )
{
	m_pModel->Draw();
}

bool CBaseModeled::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_pModel->ShouldDraw();
}

void CBaseModeled::SetModel( CModel *pModel )
{
	m_pModel = pModel;
}

CModel *CBaseModeled::GetModel( void )
{
	return m_pModel;
}