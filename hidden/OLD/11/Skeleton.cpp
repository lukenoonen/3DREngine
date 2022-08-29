#include "Skeleton.h"

CSkeleton::CSkeleton( SSkeletonNode *pSkeletonNode, unsigned int uiSkeletonBonesCount, const char *sPath ) : BaseClass( sPath )
{
	m_pSkeletonNode = pSkeletonNode;
	m_uiSkeletonBonesCount = uiSkeletonBonesCount;
}

CSkeleton::~CSkeleton()
{
	delete m_pSkeletonNode;
}

EAssetType CSkeleton::GetAssetType( void ) const
{
	return EAssetType::t_skeleton;
}

void CSkeleton::SetUpBoneTransforms( std::vector<glm::mat4> &matBoneTransforms )
{
	matBoneTransforms.resize( m_uiSkeletonBonesCount );
}

#include <iostream>

void CSkeleton::UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors )
{
	std::vector<float> flAdjustedAnimationTransitionFactors;
	for (unsigned int i = 0; i < flAnimationTransitionFactors.size(); i++)
	{
		float flAdjustedAnimationTransitionFactor = (flAnimationTransitionFactors[i] - 1.0f) / flAnimationTransitionFactors[i];
		flAdjustedAnimationTransitionFactor = 1.0f / (1.0f + flAdjustedAnimationTransitionFactor * flAdjustedAnimationTransitionFactor);
		flAdjustedAnimationTransitionFactors.push_back( flAdjustedAnimationTransitionFactor );
	}

	ReadNodeHierarchy( matBoneTransforms, pAnimations, flAnimationTimes, flAdjustedAnimationTransitionFactors, m_pSkeletonNode, g_matIdentity );
}

void CSkeleton::ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors, SSkeletonNode *pSkeletonNode, const glm::mat4 &matParentTransform )
{
	glm::vec3 vec3Position;
	glm::quat qRotation;
	glm::vec3 vec3Scale;
	
	pAnimations[0]->GetTransform( vec3Position, qRotation, vec3Scale, pSkeletonNode->uiIndex, flAnimationTimes[0] );

	for (unsigned int i = 0; i < (unsigned int)flAnimationTransitionFactors.size(); i++)
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

	for (unsigned int i = 0; i < (unsigned int)pSkeletonNode->pChildren.size(); i++)
		ReadNodeHierarchy( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors, pSkeletonNode->pChildren[i], matGlobalTransformation );
}