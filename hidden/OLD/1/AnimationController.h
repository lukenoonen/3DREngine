#ifndef ANIMATIONCONTROLLER_H
#define ANIMATIONCONTROLLER_H

#include "Global.h"
#include "BaseController.h"
#include "Animation.h"

typedef CBaseControllerSequence<CAnimation> CAnimationControllerSequence;
typedef CBaseController<CAnimation> CAnimationController;

/*#include "Global.h"
#include "Animation.h"

class CAnimationController
{
public:
	DECLARE_CLASS_NOBASE( CAnimationController );

	CAnimationController( const std::vector<CAnimation *> &pAnimations, const std::vector<unsigned int> &uiNextAnimations );

	bool ShouldAnimate( void );

	void PlayAnimation( unsigned int uiIndex );
	void QueueAnimation( unsigned int uiIndex );
	void UpdateAnimation( void );

	CAnimation *GetCurrentAnimation( void );
	float GetCurrentTime( void );

	void SetAnimationRate( float flAnimationRate );

	void SetBoneTransformSize( unsigned int uiSize );
	void SetBoneTransformation( unsigned int uiIndex, const glm::mat4 &matTransform );
	const std::vector<glm::mat4> &GetBoneTransforms( void );

private:
	std::vector<CAnimation *> m_pAnimations;
	std::vector<unsigned int> m_uiNextAnimations;

	unsigned int m_uiCurrentAnimation;
	float m_flCurrentAnimationTime;

	bool m_bAnimationQueued;
	unsigned int m_uiQueuedAnimation;

	float m_flAnimationRate;

	std::vector<glm::mat4> m_matBoneTransform;
};*/

#endif // ANIMATIONCONTROLLER_H