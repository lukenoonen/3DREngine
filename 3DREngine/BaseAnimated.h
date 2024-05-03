#ifndef BASEANIMATED_H
#define BASEANIMATED_H

#include "Global.h"
#include "BaseModeled.h"

DEFINE_ENTITY_FLAG( fl_animate, 4 )
DEFINE_ENTITY_FLAG( fl_updateanimation, 5 )

class CBaseAnimated : public CBaseModeled
{
public:
	DECLARE_CLASS( CBaseAnimated, CBaseModeled )

	DECLARE_DATADESC()

	CBaseAnimated();

	virtual bool Init( void );

	virtual void PreThink( void );
	virtual void PreDraw( void );
	virtual void PostDraw( void );

	void TransitionAnimation( unsigned int uiAnimationIndex, float flAnimationTransitionTime );
	void SetAnimation( unsigned int uiAnimationIndex );
		
private:
	std::vector<unsigned int> m_uiAnimations;
	std::vector<float> m_flAnimationTimes;
	std::vector<float> m_flAnimationTransitionFactors;
	std::vector<float> m_flAnimationTransitionTimes;

	float m_flAnimationTimeScale;

	std::vector<glm::mat4> m_matBoneTransforms;
};

#endif // BASEANIMATED_H