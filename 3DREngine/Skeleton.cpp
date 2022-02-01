#include "Skeleton.h"

CSkeletonBone::CSkeletonBone( unsigned int uiIndex, const glm::mat4 &matOffset )
{
	m_uiIndex = uiIndex;

	m_matOffset = matOffset;
}

unsigned int CSkeletonBone::GetIndex( void ) const
{
	return m_uiIndex;
}

const glm::mat4 &CSkeletonBone::GetOffset( void ) const
{
	return m_matOffset;
}

CSkeletonNode::CSkeletonNode( unsigned int uiIndex, CSkeletonBone *pSkeletonBone )
{
	m_uiIndex = uiIndex;

	m_pSkeletonBone = pSkeletonBone;
}

CSkeletonNode::~CSkeletonNode()
{
	for (unsigned int i = 0; i < (unsigned int)m_pChildren.size(); i++)
		delete m_pChildren[i];

	if (m_pSkeletonBone)
		delete m_pSkeletonBone;
}

unsigned int CSkeletonNode::GetIndex( void ) const
{
	return m_uiIndex;
}

CSkeletonBone *CSkeletonNode::GetSkeletonBone( void ) const
{
	return m_pSkeletonBone;
}

void CSkeletonNode::SetChildren( const std::vector<CSkeletonNode *> &pChildren )
{
	m_pChildren = pChildren;
}

unsigned int CSkeletonNode::GetChildrenCount( void ) const
{
	return (unsigned int)m_pChildren.size();
}

CSkeletonNode *CSkeletonNode::GetChild( unsigned int uiIndex ) const
{
	return m_pChildren[uiIndex];
}

CSkeleton::CSkeleton( CSkeletonNode *pSkeletonNode, unsigned int uiSkeletonBonesCount, const char *sPath ) : BaseClass( sPath )
{
	m_pSkeletonNode = pSkeletonNode;
	m_uiSkeletonBonesCount = uiSkeletonBonesCount;
}

CSkeleton::~CSkeleton()
{
	delete m_pSkeletonNode;
}

void CSkeleton::SetUpBoneTransforms( std::vector<glm::mat4> &matBoneTransforms )
{
	matBoneTransforms.resize( m_uiSkeletonBonesCount );
}

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

void CSkeleton::ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors, CSkeletonNode *pSkeletonNode, const glm::mat4 &matParentTransform )
{
	glm::mat4 matGlobalTransformation = matParentTransform;

	glm::vec3 vec3Position = g_vec3Zero;
	glm::quat qRotation = glm::quat( 1.0f, 0.0f, 0.0f, 0.0f );
	glm::vec3 vec3Scale = g_vec3One;

	CAnimationChannel *pAnimationChannel = pAnimations[0]->GetAnimationChannel( pSkeletonNode->GetIndex() );
	if (pAnimationChannel)
	{
		pAnimationChannel->CalcInterpolatedPosition( vec3Position, flAnimationTimes[0] );
		pAnimationChannel->CalcInterpolatedRotation( qRotation, flAnimationTimes[0] );
		pAnimationChannel->CalcInterpolatedScale( vec3Scale, flAnimationTimes[0] );
	}

	for (unsigned int i = 0; i < (unsigned int)flAnimationTransitionFactors.size(); i++)
	{
		glm::vec3 vec3PositionTransition = g_vec3Zero;
		glm::quat qRotationTransition = glm::quat( 1.0f, 0.0f, 0.0f, 0.0f );
		glm::vec3 vec3ScaleTransition = g_vec3One;

		CAnimationChannel *pAnimationChannelTransition = pAnimations[i + 1]->GetAnimationChannel( pSkeletonNode->GetIndex() );
		if (pAnimationChannelTransition)
		{
			pAnimationChannelTransition->CalcInterpolatedPosition( vec3PositionTransition, flAnimationTimes[i + 1] );
			pAnimationChannelTransition->CalcInterpolatedRotation( qRotationTransition, flAnimationTimes[i + 1] );
			pAnimationChannelTransition->CalcInterpolatedScale( vec3ScaleTransition, flAnimationTimes[i + 1] );
		}

		vec3Position = (1.0f - flAnimationTransitionFactors[i]) * vec3Position + flAnimationTransitionFactors[i] * vec3PositionTransition;
		qRotation = glm::normalize( glm::slerp( qRotation, qRotationTransition, flAnimationTransitionFactors[i] ) );
		vec3Scale = (1.0f - flAnimationTransitionFactors[i]) * vec3Scale + flAnimationTransitionFactors[i] * vec3ScaleTransition;
	}

	matGlobalTransformation = matGlobalTransformation * glm::translate( g_matIdentity, vec3Position ) * glm::toMat4( qRotation ) * glm::scale( g_matIdentity, vec3Scale );

	CSkeletonBone *pSkeletonBone = pSkeletonNode->GetSkeletonBone();
	if (pSkeletonBone)
		matBoneTransforms[pSkeletonBone->GetIndex()] = matGlobalTransformation * pSkeletonBone->GetOffset();

	for (unsigned i = 0; i < pSkeletonNode->GetChildrenCount(); i++)
		ReadNodeHierarchy( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors, pSkeletonNode->GetChild( i ), matGlobalTransformation );
}