#ifndef ANIMATION_H
#define ANIMATION_H

#include "Global.h"
#include "BaseAsset.h"
#include "AnimationResource.h"

class CAnimation : public CBaseAsset
{
public:
	DECLARE_CLASS( CAnimation, CBaseAsset )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CAnimation();

	float GetTime( void ) const;

	void GetTransform( glm::vec3 &vec3Position, glm::quat &qRotation, glm::vec3 &vec3Scale, unsigned int uiIndex, float flAnimationTime ) const;

private:
	CAnimationResource *m_pAnimationResource;
};

#endif // ANIMATION_H