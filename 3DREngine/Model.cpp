#include "Model.h"
#include "GlobalManager.h"
#include "AssetManager.h"

CModel::CModel( const std::vector<CMesh *> &pMeshes, const std::vector<CAnimation *> &pAnimations, CSkeleton *pSkeleton, const char *sPath ) : BaseClass( sPath )
{
	m_pMeshes = pMeshes;
	m_pAnimations = pAnimations;
	m_pSkeleton = pSkeleton;

	for (unsigned int i = 0; i < (unsigned int)m_pMeshes.size(); i++)
		m_pMeshes[i]->Activate();

	for (unsigned int i = 0; i < (unsigned int)m_pAnimations.size(); i++)
		m_pAnimations[i]->Activate();

	if (m_pSkeleton)
		m_pSkeleton->Activate();
	
	ERenderPass eRenderPass = pRenderManager->GetRenderPass();
	for (EBaseEnum i = 0; i < (EBaseEnum)ERenderPass::i_count; i++)
	{
		pRenderManager->SetRenderPass( (ERenderPass)i );
		m_bShouldDraw[i] = false;
		for (unsigned int j = 0; j < (unsigned int)m_pMeshes.size(); j++)
		{
			if (m_pMeshes[j]->ShouldDraw())
			{
				m_bShouldDraw[i] = true;
				break;
			}
		}
	}
	pRenderManager->SetRenderPass( eRenderPass );

	m_bIsAnimated = m_pSkeleton != NULL && !m_pAnimations.empty();
}

CModel::~CModel()
{
	for (unsigned int i = 0; i < (unsigned int)m_pMeshes.size(); i++)
	{
		m_pMeshes[i]->Inactivate();
		pAssetManager->CheckMesh( m_pMeshes[i] );
	}

	for (unsigned int i = 0; i < (unsigned int)m_pAnimations.size(); i++)
	{
		m_pAnimations[i]->Inactivate();
		pAssetManager->CheckAnimation( m_pAnimations[i] );
	}

	if (m_pSkeleton)
	{
		m_pSkeleton->Inactivate();
		pAssetManager->CheckSkeleton( m_pSkeleton );
	}
}

bool CModel::ShouldDraw( void ) const
{
	return m_bShouldDraw[(EBaseEnum)pRenderManager->GetRenderPass()];
}

void CModel::Draw( void )
{
	for (unsigned int i = 0; i < (unsigned int)m_pMeshes.size(); i++)
		m_pMeshes[i]->Draw();
}

bool CModel::IsAnimated( void ) const
{
	return m_bIsAnimated;
}

CAnimation *CModel::GetAnimation( unsigned int uiIndex ) const
{
	if (uiIndex < (unsigned int)m_pAnimations.size())
		return m_pAnimations[uiIndex];

	return NULL;
}

void CModel::SetUpBoneTransforms( std::vector<glm::mat4> &matBoneTransforms )
{
	m_pSkeleton->SetUpBoneTransforms( matBoneTransforms );
}

void CModel::UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors )
{
	m_pSkeleton->UpdateAnimation( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors );
}