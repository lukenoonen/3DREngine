#ifndef SKELETON_H
#define SKELETON_H

#include "Global.h"
#include "BaseAsset.h"
#include "Animation.h"

class CSkeletonBone
{
public:
	DECLARE_CLASS_NOBASE( CSkeletonBone );

	CSkeletonBone( unsigned int uiIndex, const glm::mat4 &matOffset );

	unsigned int GetIndex( void ) const;

	const glm::mat4 &GetOffset( void ) const;

private:
	unsigned int m_uiIndex;

	glm::mat4 m_matOffset;
};

class CSkeletonNode
{
public:
	DECLARE_CLASS_NOBASE( CSkeletonNode );

	CSkeletonNode( unsigned int uiIndex, CSkeletonBone *pSkeletonBone );
	~CSkeletonNode();

	unsigned int GetIndex( void ) const;

	CSkeletonBone *GetSkeletonBone( void ) const;

	void SetChildren( const std::vector<CSkeletonNode *> &pChildren );

	unsigned int GetChildrenCount( void ) const;
	CSkeletonNode *GetChild( unsigned int uiIndex ) const;

private:
	unsigned int m_uiIndex;

	CSkeletonBone *m_pSkeletonBone;

	std::vector<CSkeletonNode *> m_pChildren;
};

class CSkeleton : public CBaseAsset
{
public:
	DECLARE_CLASS( CSkeleton, CBaseAsset );

	CSkeleton( CSkeletonNode *pSkeletonNode, unsigned int uiSkeletonBonesCount, const char *sPath );
	virtual ~CSkeleton();

	void SetUpBoneTransforms( std::vector<glm::mat4> &matBoneTransforms );
	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, CAnimation *pAnimation, float flAnimationTime );
	
private:
	void ReadNodeHierarchy( std::vector<glm::mat4> &matBoneTransforms, CAnimation *pAnimation, float flAnimationTime, CSkeletonNode *pSkeletonNode, const glm::mat4 &matParentTransform );

private:
	CSkeletonNode *m_pSkeletonNode;
	unsigned int m_uiSkeletonBonesCount;
};

#endif // SKELETON_H