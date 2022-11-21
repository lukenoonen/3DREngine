#ifndef RIGGING_H
#define RIGGING_H

#include "Global.h"
#include "BaseAsset.h"
#include "Skeleton.h"
#include "Animation.h" 

class CRigging : public CBaseAsset
{
public:
	DECLARE_CLASS( CRigging, CBaseAsset )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CRigging();

	unsigned int GetBonesCount( void );
	float GetAnimationTime( unsigned int uiAnimationIndex );
	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<unsigned int> &uiAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors );

private:
	CHandle<CSkeleton> m_hSkeleton;
	std::vector<CHandle<CAnimation>> m_hAnimations;
};

#endif // RIGGING_H