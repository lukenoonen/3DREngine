#include "Model.h"

DEFINE_DATADESC( CModel )

	DEFINE_FIELD( LinkedVectorDataField, CHandle<CGeometry>, m_hGeometry, "geometry", FL_REQUIRED )
	DEFINE_FIELD( LinkedVectorDataField, CHandle<CBaseMaterial>, m_hMaterials, "materials", FL_REQUIRED )
	DEFINE_FIELD( LinkedDataField, CHandle<CRigging>, m_hRigging, "rigging", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CModel, model )

CModel::CModel()
{

}

bool CModel::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (m_hGeometry.size() != m_hMaterials.size())
		return false;

	ERenderPass eRenderPass = pRenderManager->GetRenderPass();
	for (EBaseEnum i = 0; i < (EBaseEnum)ERenderPass::i_count; i++)
	{
		pRenderManager->SetRenderPass( (ERenderPass)i );
		m_bShouldDraw[i] = false;
		for (unsigned int j = 0; j < m_hMaterials.size(); j++)
		{
			if (m_hMaterials[j]->ShouldApply())
			{
				m_bShouldDraw[i] = true;
				break;
			}
		}
	}
	pRenderManager->SetRenderPass( eRenderPass );

	return true;
}

bool CModel::ShouldDraw( void )
{
	return m_bShouldDraw[(EBaseEnum)pRenderManager->GetRenderPass()];
}

void CModel::Draw( void )
{
	for (unsigned int i = 0; i < m_hGeometry.size(); i++)
	{
		if (m_hMaterials[i]->ShouldApply())
		{
			m_hMaterials[i]->Apply();
			m_hGeometry[i]->Draw();
		}
	}
}

bool CModel::IsAnimated( void ) const
{
	return m_hRigging != NULL;
}

unsigned int CModel::GetBonesCount( void ) const
{
	return m_hRigging->GetBonesCount();
}

float CModel::GetAnimationTime( unsigned int uiAnimationIndex )
{
	return m_hRigging->GetAnimationTime( uiAnimationIndex );
}

void CModel::UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<unsigned int> &uiAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors )
{
	m_hRigging->UpdateAnimation( matBoneTransforms, uiAnimations, flAnimationTimes, flAnimationTransitionFactors );
}