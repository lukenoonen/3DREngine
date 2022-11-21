#ifndef MODEL_H
#define MODEL_H

#include "Global.h"
#include "BaseAsset.h"
#include "BaseMaterial.h"
#include "Rigging.h"
#include "Geometry.h"

class CModel : public CBaseAsset
{
public:
	DECLARE_CLASS( CModel, CBaseAsset )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CModel();

	virtual bool Init( void );

	bool ShouldDraw( void );
	void Draw( void );

	bool IsAnimated( void ) const;
	unsigned int GetBonesCount( void ) const;
	float GetAnimationTime( unsigned int uiAnimationIndex );
	void UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<unsigned int> &uiAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors );

private:
	std::vector<CHandle<CGeometry>> m_hGeometry;
	std::vector<CHandle<CBaseMaterial>> m_hMaterials;
	CHandle<CRigging> m_hRigging;

	bool m_bShouldDraw[(EBaseEnum)ERenderPass::i_count];
};

#endif // MODEL_H