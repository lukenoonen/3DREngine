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
	void SetAnimationIndex( unsigned int uiAnimationIndex );

	void SetAnimationTime( float flAnimationTime );
	void SetAnimationTimeScale( float flAnimationTimeScale );

private:
	bool m_bAnimate;
	bool m_bUpdateAnimation;
	bool m_bValidAnimationIndex;
	unsigned int m_uiAnimationIndex;

	float m_flAnimationTime;
	float m_flAnimationTimeScale;

	std::vector<glm::mat4> m_matBoneTransforms;
};

#endif // BASEANIMATED_H