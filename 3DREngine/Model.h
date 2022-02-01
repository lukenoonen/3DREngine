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

	CAnimation *GetAnimation( unsigned int uiIndex ) const;

	void SetUpBoneTransforms( std::vector<glm::mat4> &matBoneTransforms );
	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<CAnimation *> &pAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors );
	
private:
	std::vector<CMesh *> m_pMeshes;
	std::vector<CAnimation *> m_pAnimations;
	CSkeleton *m_pSkeleton;
	bool m_bShouldDraw[(EBaseEnum)ERenderPass::i_count];
	bool m_bIsAnimated;
};

#endif // MODEL_H