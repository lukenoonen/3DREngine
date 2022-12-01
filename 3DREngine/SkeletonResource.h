#ifndef SKELETONRESOURCE_H
#define SKELETONRESOURCE_H

#include "Global.h"
#include "BaseResource.h"
#include "SkeletonResourceData.h"
#include "Animation.h"

class CSkeletonResource : public CBaseResource
{
public:
	DECLARE_CLASS( CSkeletonResource, CBaseResource )

	CSkeletonResource( SSkeletonResourceData &sData, const char *sName );
	virtual ~CSkeletonResource();

	unsigned int GetBonesCount( void ) const;

	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors ) const;

private:
	void ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors, SSkeletonNode *pSkeletonNode, const glm::mat4 &matParentTransform ) const;

private:
	SSkeletonNode *m_pSkeletonNode;
	unsigned int m_uiSkeletonBonesCount;
};

bool UTIL_Write( CFile *pFile, CSkeletonResource *&pData );
bool UTIL_Read( CFile *pFile, CSkeletonResource *&pData );
bool UTIL_GetValue( const CTextItem *pTextItem, CSkeletonResource *&pValue );

#endif // SKELETONRESOURCE_H