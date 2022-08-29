#include "MaterialController.h"

CMaterialController::CMaterialController( const std::vector<CMaterial *> &pMaterials )
{
	m_pMaterials = pMaterials;

	m_uiSummedDrawFlags = 0;
	for (unsigned int i = 0; i < m_pMaterials.size(); i++)
		m_uiSummedDrawFlags |= m_pMaterials[i]->GetDrawFlags();
}

bool CMaterialController::Use( void )
{
	return m_pMaterials[0]->Use();
}

bool CMaterialController::Use( unsigned int uiIndex )
{
	return m_pMaterials[uiIndex]->Use();
}

bool CMaterialController::ShouldDraw( unsigned int uiDrawFlags ) const
{
	return (m_uiSummedDrawFlags & uiDrawFlags) != 0;
}

bool CMaterialController::ShouldDraw( unsigned int uiIndex, unsigned int uiDrawFlags ) const
{
	return m_pMaterials[uiIndex]->ShouldDraw( uiDrawFlags );
}

unsigned int CMaterialController::GetMaterialCount( void ) const
{
	return (unsigned int)m_pMaterials.size();
}