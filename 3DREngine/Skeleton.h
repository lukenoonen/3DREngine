#ifndef SKELETON_H
#define SKELETON_H

#include "Global.h"
#include "BaseAsset.h"
#include "AssetManager.h"
#include "Animation.h"

class CSkeleton : public CBaseAsset
{
public:
	DECLARE_CLASS( CSkeleton, CBaseAsset )

	CSkeleton();
	virtual ~CSkeleton();

	virtual bool Init( void );

	unsigned int GetBonesCount( void );

	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors );

private:
	void ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors, SSkeletonNode *pSkeletonNode, const glm::mat4 &matParentTransform );

private:
	unsigned int m_uiSkeletonBonesCount;
	SSkeletonNode *m_pSkeletonNode;
};

#endif // SKELETON_H