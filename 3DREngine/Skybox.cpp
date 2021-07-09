#include "Skybox.h"
#include "MaterialManager.h"
#include "ModelManager.h"
#include "RenderManager.h"

CSkybox::CSkybox( const char *sMaterialPath, bool bShouldDraw, bool bActive ) : BaseClass( bShouldDraw, bActive )
{
	m_pMaterialController = pMaterialManager->GetMaterialController( sMaterialPath );
	m_pModel = pModelManager->GetGenericModel( GENERICMODELS_INVERSECUBE );
}

void CSkybox::Draw( void )
{
	m_pModel->Draw( m_pMaterialController );
}

bool CSkybox::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_pMaterialController->ShouldDraw( pRenderManager->GetDrawFlags() );
}