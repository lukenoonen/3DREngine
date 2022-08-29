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

	CRigging();

	virtual bool Init( void );

	unsigned int GetBonesCount( void );
	float GetAnimationTime( unsigned int uiAnimationIndex );
	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<unsigned int> &uiAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors );

	void SetSkeleton( CSkeleton *pSkeleton );

	void AddAnimation( CAnimation *pAnimation );

private:
	CSkeleton *m_pSkeleton;
	std::vector<CAnimation *> m_pAnimations;
};

#endif // RIGGING_H