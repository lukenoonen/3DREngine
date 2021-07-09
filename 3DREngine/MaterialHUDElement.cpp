#include "MaterialHUDElement.h"
#include "MaterialManager.h"
#include "RenderManager.h"

CMaterialHUDElement::CMaterialHUDElement( const char *sMaterialPath, const glm::vec2 &vecPosition, const glm::vec2 &vecScale, float flPriority, bool bShouldDraw, bool bActive ) : BaseClass( vecPosition, vecScale, flPriority, bShouldDraw, bActive )
{
	m_pMaterialController = pMaterialManager->GetMaterialController( sMaterialPath );
}

void CMaterialHUDElement::Draw( void )
{
	m_pMaterialController->Use();
	BaseClass::Draw();
}

bool CMaterialHUDElement::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_pMaterialController->ShouldDraw( pRenderManager->GetDrawFlags() );
}