#include "BaseModeled.h"
#include "TimeManager.h"
#include "RenderManager.h"

DEFINE_DATADESC( CBaseModeled )

	DEFINE_FIELD( LinkedDataField, CHandle<CModel>, m_hModel, "shadowcamera", FL_REQUIRED )

END_DATADESC()

CBaseModeled::CBaseModeled()
{

}

bool CBaseModeled::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (!m_hModel)
		return false;

	return true;
}

void CBaseModeled::PreDraw( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 0, &GetModelMatrix() ); // TODO: change "model" matrix to better name
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 1, &GetModelMatrixInverse() );
}

void CBaseModeled::Draw( void )
{
	m_hModel->Draw();
}

bool CBaseModeled::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_hModel->ShouldDraw();
}

CModel *CBaseModeled::GetModel( void )
{
	return m_hModel;
}