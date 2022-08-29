#include "Skeleton.h"
#include "AssetManager.h"

CSkeleton::CSkeleton()
{
	m_pSkeletonNode = NULL;
}

CSkeleton::~CSkeleton()
{
	if (m_pSkeletonNode)
		delete m_pSkeletonNode;
}

bool CSkeleton::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (!pAssetManager->LoadSkeleton( GetFilePath(), m_pSkeletonNode, m_uiSkeletonBonesCount ))
		return false;

	return true;
}

unsigned int CSkeleton::GetBonesCount( void )
{
	return m_uiSkeletonBonesCount;
}

void CSkeleton::UpdateAnimation(std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors)
{
	ReadNodeHierarchy( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors, m_pSkeletonNode, g_matIdentity );
}

// TODO: see if there is a better way to do this
void CSkeleton::ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors, SSkeletonNode *pSkeletonNode, const glm::mat4 &matParentTransform )
{
	glm::vec3 vec3Position;
	glm::quat qRotation;
	glm::vec3 vec3Scale;

	pAnimations[0]->GetTransform( vec3Position, qRotation, vec3Scale, pSkeletonNode->usIndex, flAnimationTimes[0] );

	for (unsigned int i = 0; i < flAnimationTransitionFactors.size(); i++)
	{
		glm::vec3 vec3PositionTransition;
		glm::quat qRotationTransition;
		glm::vec3 vec3ScaleTransition;

		pAnimations[i + 1]->GetTransform( vec3PositionTransition, qRotationTransition, vec3ScaleTransition, pSkeletonNode->usIndex, flAnimationTimes[i + 1] );

		vec3Position = (1.0f - flAnimationTransitionFactors[i]) * vec3Position + flAnimationTransitionFactors[i] * vec3PositionTransition;
		qRotation = glm::normalize( glm::slerp( qRotation, qRotationTransition, flAnimationTransitionFactors[i] ) );
		vec3Scale = (1.0f - flAnimationTransitionFactors[i]) * vec3Scale + flAnimationTransitionFactors[i] * vec3ScaleTransition;
	}

	glm::mat4 matGlobalTransformation = matParentTransform * glm::translate( g_matIdentity, vec3Position ) * glm::toMat4( qRotation ) * glm::scale( g_matIdentity, vec3Scale );

	if (pSkeletonNode->pSkeletonBone)
		matBoneTransforms[pSkeletonNode->pSkeletonBone->usIndex] = matGlobalTransformation * pSkeletonNode->pSkeletonBone->matOffset;

	for (unsigned int i = 0; i < pSkeletonNode->pChildren.size(); i++)
		ReadNodeHierarchy( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors, pSkeletonNode->pChildren[i], matGlobalTransformation );
}