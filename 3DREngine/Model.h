#ifndef MODEL_H
#define MODEL_H

#include "Global.h"
#include "BaseAsset.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Animation.h"
#include "RenderManager.h"

class CModel : public CBaseAsset
{
public:
	DECLARE_CLASS( CModel, CBaseAsset );

	CModel( const std::vector<CMesh *> &pMeshes, const std::vector<CAnimation *> &pAnimations, CSkeleton *pSkeleton, const char *sPath );
	virtual ~CModel();

	bool ShouldDraw( void ) const;

	void Draw( void );

	bool IsAnimated( void ) const;
	bool IsValidAnimation( unsigned int uiIndex ) const;

	void SetUpBoneTransforms( std::vector<glm::mat4> &matBoneTransforms );
	float UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, unsigned int uiAnimationIndex, float flAnimationTime, float flAnimationTimeScale );
	
private:
	std::vector<CMesh *> m_pMeshes;
	std::vector<CAnimation *> m_pAnimations;
	CSkeleton *m_pSkeleton;
	bool m_bShouldDraw[RENDERPASS_COUNT];
	bool m_bIsAnimated;
};

#endif // MODEL_H