#include "Model.h"

CModel::CModel()
{
	m_pRigging = NULL;
}

bool CModel::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (m_pGeometry.empty())
		return false;

	if (m_pMaterials.empty())
		return false;

	if (m_pGeometry.size() != m_pMaterials.size())
		return false;

	ERenderPass eRenderPass = pRenderManager->GetRenderPass();
	for (EBaseEnum i = 0; i < (EBaseEnum)ERenderPass::i_count; i++)
	{
		pRenderManager->SetRenderPass( (ERenderPass)i );
		m_bShouldDraw[i] = false;
		for (unsigned int j = 0; j < m_pMaterials.size(); j++)
		{
			if (m_pMaterials[j]->ShouldApply())
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
	for (unsigned int i = 0; i < m_pGeometry.size(); i++)
	{
		if (m_pMaterials[i]->ShouldApply())
		{
			m_pMaterials[i]->Apply();
			m_pGeometry[i]->Draw();
		}
	}
}

bool CModel::IsAnimated( void )
{
	return m_pRigging != NULL;
}

unsigned int CModel::GetBonesCount( void )
{
	return m_pRigging->GetBonesCount();
}

float CModel::GetAnimationTime( unsigned int uiAnimationIndex )
{
	return m_pRigging->GetAnimationTime( uiAnimationIndex );
}

void CModel::UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<unsigned int> &uiAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors )
{
	m_pRigging->UpdateAnimation( matBoneTransforms, uiAnimations, flAnimationTimes, flAnimationTransitionFactors );
}

void CModel::AddGeometry( CGeometry *pGeometry )
{
	m_pGeometry.push_back( pGeometry );
}

void CModel::AddMaterial( CBaseMaterial *pMaterial )
{
	m_pMaterials.push_back( pMaterial );
}

void CModel::SetRigging( CRigging *pRigging )
{
	m_pRigging = pRigging;
}