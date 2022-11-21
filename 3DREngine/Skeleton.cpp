#include "Skeleton.h"

DEFINE_DATADESC( CSkeleton )

	DEFINE_FIELD( DataField, CSkeletonResource *, m_pSkeletonResource, "skeletonresource", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CSkeleton, skeleton )

CSkeleton::CSkeleton()
{

}

unsigned int CSkeleton::GetBonesCount( void ) const
{
	return m_pSkeletonResource->GetBonesCount();
}

void CSkeleton::UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors )
{
	m_pSkeletonResource->UpdateAnimation( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors );
}