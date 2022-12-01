#include "Animation.h"

DEFINE_DATADESC( CAnimation )

	DEFINE_FIELD( DataField, CAnimationResource *, m_pAnimationResource, "animationresource", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CAnimation, asset_animation )

CAnimation::CAnimation()
{

}

float CAnimation::GetTime( void ) const
{
	return m_pAnimationResource->GetTime();
}

void CAnimation::GetTransform( glm::vec3 &vec3Position, glm::quat &qRotation, glm::vec3 &vec3Scale, unsigned int uiIndex, float flAnimationTime ) const
{
	m_pAnimationResource->GetTransform( vec3Position, qRotation, vec3Scale, uiIndex, flAnimationTime );
}