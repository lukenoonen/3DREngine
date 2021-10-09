#ifndef BASEANIMATED_H
#define BASEANIMATED_H

#include "Global.h"
#include "BaseModeled.h"
#include "Skeleton.h"
#include "Animation.h"

class CBaseAnimated : public CBaseModeled
{
public:
	DECLARE_CLASS( CBaseAnimated, CBaseModeled );

	CBaseAnimated( const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PostThink( void );
	virtual void PreDraw( void );

	virtual void SetModel( CModel *pModel );

	void SetAnimate( bool bAnimate );
	void SetUpdateAnimation( bool bUpdateAnimation );

	void TransitionAnimation( unsigned int uiAnimationIndex, float flAnimationTransitionTime );
	void SetAnimation( unsigned int uiAnimationIndex );

	void SetAnimationTimeScale( float flAnimationTimeScale );
		
private:
	bool m_bAnimate;
	bool m_bUpdateAnimation;

	std::vector<CAnimation *> m_pAnimations;
	std::vector<float> m_flAnimationTimes;
	std::vector<float> m_flAnimationTransitionFactors;
	std::vector<float> m_flAnimationTransitionTimes;

	/*
	unsigned int m_uiAnimationIndex;
	unsigned int m_uiAnimationIndexTransition;
	float m_flAnimationTime;
	float m_flAnimationTimeTransition;
	float m_flAnimationTransitionFactor;
	float m_flAnimationTransitionTime;
	*/

	float m_flAnimationTimeScale;

	std::vector<glm::mat4> m_matBoneTransforms;
};

#endif // BASEANIMATED_H