#include "SkeletonResource.h"
#include "ResourceManager.h"

CSkeletonResource::CSkeletonResource( SSkeletonResourceData &sData, const char *sName ) : BaseClass( sName )
{
	m_pSkeletonNode = sData.pSkeletonNode;
	m_uiSkeletonBonesCount = sData.uiSkeletonBonesCount;
}

CSkeletonResource::~CSkeletonResource()
{
	if (m_pSkeletonNode)
		delete m_pSkeletonNode;
}

unsigned int CSkeletonResource::GetBonesCount( void ) const
{
	return m_uiSkeletonBonesCount;
}

void CSkeletonResource::UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors ) const
{
	ReadNodeHierarchy( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors, m_pSkeletonNode, g_matIdentity );
}

// TODO: see if there is a better way to do this
void CSkeletonResource::ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors, SSkeletonNode *pSkeletonNode, const glm::mat4 &matParentTransform ) const
{
	glm::vec3 vec3Position;
	glm::quat qRotation;
	glm::vec3 vec3Scale;

	pAnimations[0]->GetTransform( vec3Position, qRotation, vec3Scale, pSkeletonNode->uiIndex, flAnimationTimes[0] );

	for (unsigned int i = 0; i < flAnimationTransitionFactors.size(); i++)
	{
		glm::vec3 vec3PositionTransition;
		glm::quat qRotationTransition;
		glm::vec3 vec3ScaleTransition;

		pAnimations[i + 1]->GetTransform( vec3PositionTransition, qRotationTransition, vec3ScaleTransition, pSkeletonNode->uiIndex, flAnimationTimes[i + 1] );

		vec3Position = (1.0f - flAnimationTransitionFactors[i]) * vec3Position + flAnimationTransitionFactors[i] * vec3PositionTransition;
		qRotation = glm::normalize( glm::slerp( qRotation, qRotationTransition, flAnimationTransitionFactors[i] ) );
		vec3Scale = (1.0f - flAnimationTransitionFactors[i]) * vec3Scale + flAnimationTransitionFactors[i] * vec3ScaleTransition;
	}

	glm::mat4 matGlobalTransformation = matParentTransform * glm::translate( g_matIdentity, vec3Position ) * glm::toMat4( qRotation ) * glm::scale( g_matIdentity, vec3Scale );

	if (pSkeletonNode->pSkeletonBone)
		matBoneTransforms[pSkeletonNode->pSkeletonBone->uiIndex] = matGlobalTransformation * pSkeletonNode->pSkeletonBone->matOffset;

	for (unsigned int i = 0; i < pSkeletonNode->pChildren.size(); i++)
		ReadNodeHierarchy( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors, pSkeletonNode->pChildren[i], matGlobalTransformation );
}

bool UTIL_Write( CFile *pFile, CSkeletonResource *&pData )
{
	const char *sName = pData->GetName();
	return pFile->Write( sName );
}

bool UTIL_Read( CFile *pFile, CSkeletonResource *&pData )
{
	char *sName;
	if (!pFile->Read( sName ))
		return false;

	pData = pResourceManager->GetSkeletonResource( sName );
	delete[] sName;
	return pData != NULL;
}

bool UTIL_GetValue( const CTextItem *pTextItem, CSkeletonResource *&pValue )
{
	const char *sName;
	if (!pTextItem->GetValue( sName ))
		return false;

	pValue = pResourceManager->GetSkeletonResource( sName );
	return pValue != NULL;
}