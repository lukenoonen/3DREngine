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

	CBaseAnimated();

	virtual void Exit( void );

	virtual void PostThink( void );
	virtual void PreDraw( void );
	virtual void PostDraw( void );

	void SetSkeleton( const char *sPath );
	void AddAnimation( const char *sPath );

	void SetAnimate( bool bAnimate );
	void SetUpdateAnimation( bool bUpdateAnimation );

	void TransitionAnimation( unsigned int uiAnimationIndex, float flAnimationTransitionTime );
	void SetAnimation( unsigned int uiAnimationIndex );

	void SetAnimationTimeScale( float flAnimationTimeScale );
		
private:
	bool m_bAnimate;
	bool m_bUpdateAnimation;

	CSkeleton *m_pSkeleton;
	std::vector<CAnimation *> m_pAnimationList;

	std::vector<CAnimation *> m_pAnimations;
	std::vector<float> m_flAnimationTimes;
	std::vector<float> m_flAnimationTransitionFactors;
	std::vector<float> m_flAnimationTransitionTimes;

	float m_flAnimationTimeScale;

	std::vector<glm::mat4> m_matBoneTransforms;
};

#endif // BASEANIMATED_H