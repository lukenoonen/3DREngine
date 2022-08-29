#ifndef SKELETON_H
#define SKELETON_H

#include "Global.h"
#include "BaseAsset.h"
#include "Animation.h"

struct SSkeletonBone
{
	unsigned int uiIndex;
	glm::mat4 matOffset;
};

struct SSkeletonNode
{
	~SSkeletonNode()
	{
		delete pSkeletonBone;
		for (unsigned int i = 0; i < (unsigned int)pChildren.size(); i++)
			delete pChildren[i];
	}

	unsigned int uiIndex;
	SSkeletonBone *pSkeletonBone;
	std::vector<SSkeletonNode *> pChildren;
};

class CSkeleton : public CBaseAsset
{
public:
	DECLARE_CLASS( CSkeleton, CBaseAsset );

	CSkeleton( SSkeletonNode *pSkeletonNode, unsigned int uiSkeletonBonesCount, const char *sPath );
	virtual ~CSkeleton();

	virtual EAssetType GetAssetType( void ) const;

	void SetUpBoneTransforms( std::vector<glm::mat4> &matBoneTransforms );
	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors );
	
private:
	void ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors, SSkeletonNode *pSkeletonNode, const glm::mat4 &matParentTransform );
	
private:
	SSkeletonNode *m_pSkeletonNode;
	unsigned int m_uiSkeletonBonesCount;
};

#endif // SKELETON_H