#ifndef BASEANIMATED_H
#define BASEANIMATED_H

#include "Global.h"
#include "BaseModeled.h"

#define FL_ANIMATE			(1<<4)
#define FL_UPDATEANIMATION	(1<<5)

class CBaseAnimated : public CBaseModeled
{
public:
	DECLARE_CLASS( CBaseAnimated, CBaseModeled )

	DECLARE_DATADESC()

	CBaseAnimated();

	virtual bool Init( void );

	virtual void PostThink( void );
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