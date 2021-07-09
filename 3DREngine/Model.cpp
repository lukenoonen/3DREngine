#include "Model.h"
#include <UTIL.h>
#include <GLFW/glfw3.h>
#include "RenderManager.h"

CModel::CModel( const char *sPath, const std::vector<CMesh *> &pMeshes, const std::vector<CModelNode *> &pModelNodes, const std::vector<glm::mat4> &matBoneOffsets )
{
	m_sPath = UTIL_stredit( sPath );

	m_pMeshes = pMeshes;
	m_pModelNodes = pModelNodes;

	m_matBoneOffsets = matBoneOffsets;
}

CModel::~CModel()
{
	delete[] m_sPath;

	for (unsigned int i = 0; i < m_pMeshes.size(); i++)
		delete m_pMeshes[i];

	for (unsigned int i = 0; i < m_pModelNodes.size(); i++)
		delete m_pModelNodes[i];
}

void CModel::Draw( CMaterialController *pMaterialController )
{
	for (unsigned int i = 0; i < m_pMeshes.size(); i++)
	{
		if (pMaterialController->ShouldDraw( i, pRenderManager->GetDrawFlags() ))
		{
			pMaterialController->Use( i );
			m_pMeshes[i]->Draw();
		}
	}
}

void CModel::UpdateAnimation( CAnimationController *pAnimationController )
{
	ReadNodeHierarchy( pAnimationController, pAnimationController->GetCurrentAnimation(), pAnimationController->GetCurrentTime(), 0, glm::mat4( 1.0f ) );
}

const char *CModel::GetPath( void )
{
	return m_sPath;
}

unsigned int CModel::GetBoneTransformSize( void )
{
	return (unsigned int)m_matBoneOffsets.size();
}

void CModel::ReadNodeHierarchy( CAnimationController *pAnimationController, CAnimation *pAnimation, float flAnimationTime, unsigned int uiNodeIndex, const glm::mat4 &matParentTransform )
{
	glm::mat4 matGlobalTransformation = matParentTransform;

	bool bFoundAnimationChannel = true;
	unsigned int uiAnimationChannel = 0;

	for (unsigned int i = 0; i < pAnimation->GetAnimationChannelCount(); i++)
	{
		if (pAnimation->GetAnimationChannel( i ).uiAssociatedNode == uiNodeIndex)
		{
			bFoundAnimationChannel = true;
			uiAnimationChannel = i;
			break;
		}
	}

	if (bFoundAnimationChannel)
	{
		glm::quat qRotation;
		pAnimation->CalcInterpolatedRotation( qRotation, flAnimationTime, uiAnimationChannel );
		glm::mat4 matRotation = glm::toMat4( qRotation );

		glm::vec3 vecTranslation;
		pAnimation->CalcInterpolatedTranslation( vecTranslation, flAnimationTime, uiAnimationChannel );
		glm::mat4 matTranslation = glm::translate( glm::mat4( 1.0f ), vecTranslation );

		matGlobalTransformation = matGlobalTransformation * matTranslation * matRotation;
	}
	else
	{
		matGlobalTransformation = matGlobalTransformation * m_pModelNodes[uiNodeIndex]->GetTransformation();
	}


	if (m_pModelNodes[uiNodeIndex]->IsAssociatedWithBone())
		pAnimationController->SetBoneTransformation( m_pModelNodes[uiNodeIndex]->GetAssociatedBone(), matGlobalTransformation * m_matBoneOffsets[m_pModelNodes[uiNodeIndex]->GetAssociatedBone()] );
	
	for (unsigned i = 0; i < m_pModelNodes[uiNodeIndex]->GetChildrenCount(); i++)
		ReadNodeHierarchy( pAnimationController, pAnimation, flAnimationTime, m_pModelNodes[uiNodeIndex]->GetChildren( i ), matGlobalTransformation );
}