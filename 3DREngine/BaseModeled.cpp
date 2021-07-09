#include "BaseModeled.h"
#include "ModelManager.h"
#include "MaterialManager.h"
#include "GlobalManager.h"
#include "RenderManager.h"

CBaseModeled::CBaseModeled( const char *sMaterialPath, const char *sModelPath, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_pMaterialController = pMaterialManager->GetMaterialController( sMaterialPath );
	m_pModel = pModelManager->GetModel( sModelPath );
}

void CBaseModeled::PreDraw( void )
{
	pShaderManager->SetUniformBufferObject( UBO_MODEL, 0, &GetModelMatrix() );
	pShaderManager->SetUniformBufferObject( UBO_MODEL, 1, &GetModelMatrixInverse() );
}

void CBaseModeled::Draw( void )
{
	m_pModel->Draw( m_pMaterialController );
}

CModel *CBaseModeled::GetModel( void )
{
	return m_pModel;
}

bool CBaseModeled::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_pMaterialController->ShouldDraw( pRenderManager->GetDrawFlags() );
}