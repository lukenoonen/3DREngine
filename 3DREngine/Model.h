#ifndef MODEL_H
#define MODEL_H

#include "Global.h"
#include "BaseAsset.h"
#include "Geometry.h"
#include "BaseMaterial.h"
#include "Rigging.h"

class CModel : public CBaseAsset
{
public:
	DECLARE_CLASS( CModel, CBaseAsset )

	CModel();

	virtual bool Init( void );

	bool ShouldDraw( void );
	void Draw( void );

	bool IsAnimated( void );
	unsigned int GetBonesCount( void );
	float GetAnimationTime( unsigned int uiAnimationIndex );
	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<unsigned int> &uiAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors );

	void AddGeometry( CGeometry *pGeometry );

	void AddMaterial( CBaseMaterial *pMaterial );

	void SetRigging( CRigging *pRigging );

private:
	std::vector<CGeometry *> m_pGeometry;
	std::vector<CBaseMaterial *> m_pMaterials;

	CRigging *m_pRigging;

	bool m_bShouldDraw[(EBaseEnum)ERenderPass::i_count];
};

#endif // MODEL_H