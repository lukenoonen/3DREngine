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

void CSkeleton::UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, CAnimation *pAnimation, float flAnimationTime )
{
	ReadNodeHierarchy( matBoneTransforms, pAnimation, flAnimationTime, m_pSkeletonNode, glm::mat4( 1.0f ) );
}

void CSkeleton::ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransforms, CAnimation *pAnimation, float flAnimationTime, CSkeletonNode *pSkeletonNode, const glm::mat4 &matParentTransform )
{
	glm::mat4 matGlobalTransformation = matParentTransform;

	CAnimationChannel *pAnimationChannel = pAnimation->GetAnimationChannel( pSkeletonNode->GetIndex() );
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

	CSkeletonBone *pSkeletonBone = pSkeletonNode->GetSkeletonBone();
	if (pSkeletonBone)
		matBoneTransforms[pSkeletonBone->GetIndex()] = matGlobalTransformation * pSkeletonBone->GetOffset();

	for (unsigned i = 0; i < pSkeletonNode->GetChildrenCount(); i++)
		ReadNodeHierarchy( matBoneTransforms, pAnimation, flAnimationTime, pSkeletonNode->GetChild( i ), matGlobalTransformation );
}