#ifndef ANIMATION_H
#define ANIMATION_H

#include "Global.h"
#include "BaseAsset.h"
#include "AssetManager.h"

class CAnimation : public CBaseAsset
{
public:
	DECLARE_CLASS( CAnimation, CBaseAsset )

	CAnimation();
	virtual ~CAnimation();

	virtual bool Init( void );
	virtual bool Exit( void );

	float GetTime( void ) const;

	void GetTransform( glm::vec3 &vec3Position, glm::quat &qRotation, glm::vec3 &vec3Scale, unsigned int uiIndex, float flAnimationTime );

private:
	float m_flTime;
	std::vector<SAnimationChannel *> m_pAnimationChannels;
};

#endif // ANIMATION_H