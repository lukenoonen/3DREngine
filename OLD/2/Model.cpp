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

void CModel::UpdateAnimation( std::vector<glm::mat4> &matBoneTransform, CAnimation *pAnimation, float flAnimationTime )
{
	ReadNodeHierarchy( matBoneTransform, pAnimation, flAnimationTime, 0, glm::mat4( 1.0f ) );
}

const char *CModel::GetPath( void ) const
{
	return m_sPath;
}

unsigned int CModel::GetBoneTransformCount( void ) const
{
	return (unsigned int)m_matBoneOffsets.size();
}

unsigned int CModel::GetMeshCount( void ) const
{
	return (unsigned int)m_pMeshes.size();
}

void CModel::ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransform, CAnimation *pAnimation, float flAnimationTime, unsigned int uiNodeIndex, const glm::mat4 &matParentTransform )
{
	glm::mat4 matGlobalTransformation = matParentTransform;

	CAnimationChannel *pAnimationChannel = pAnimation->GetAnimationChannel( uiNodeIndex );
	if (pAnimationChannel)
	{
		glm::quat qRotation;
		pAnimationChannel->CalcInterpolatedRotation( qRotation, flAnimationTime );
		glm::mat4 matRotation = glm::toMat4( qRotation );

		glm::vec3 vecPosition;
		pAnimationChannel->CalcInterpolatedPosition( vecPosition, flAnimationTime );
		glm::mat4 matPosition = glm::translate( glm::mat4( 1.0f ), vecPosition );

		matGlobalTransformation = matGlobalTransformation * matPosition * matRotation; // TODO: figure out how to implement scale stuff (or at least figure out how its inneffective)
	}
	else
	{
		matGlobalTransformation = matGlobalTransformation * m_pModelNodes[uiNodeIndex]->GetTransformation();
	}

	if (m_pModelNodes[uiNodeIndex]->IsAssociatedWithBone())
		matBoneTransform[m_pModelNodes[uiNodeIndex]->GetAssociatedBone()] = matGlobalTransformation * m_matBoneOffsets[m_pModelNodes[uiNodeIndex]->GetAssociatedBone()];
	
	for (unsigned i = 0; i < m_pModelNodes[uiNodeIndex]->GetChildrenCount(); i++)
		ReadNodeHierarchy( matBoneTransform, pAnimation, flAnimationTime, m_pModelNodes[uiNodeIndex]->GetChildren( i ), matGlobalTransformation );
}