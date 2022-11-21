#ifndef SKELETON_H
#define SKELETON_H

#include "Global.h"
#include "BaseAsset.h"
#include "SkeletonResource.h"

class CSkeleton : public CBaseAsset
{
public:
	DECLARE_CLASS( CSkeleton, CBaseAsset )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CSkeleton();

	unsigned int GetBonesCount( void ) const;

	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors );

private:
	CSkeletonResource *m_pSkeletonResource;
};

#endif // SKELETON_H