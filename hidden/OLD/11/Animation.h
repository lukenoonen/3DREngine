#ifndef ANIMATION_H
#define ANIMATION_H

#include "Global.h"
#include "BaseAsset.h"

struct SAnimationChannel
{
	std::vector<float> flPositionTimes;
	std::vector<glm::vec3> vec3Positions;

	std::vector<float> flRotationTimes;
	std::vector<glm::quat> qRotations;

	std::vector<float> flScaleTimes;
	std::vector<glm::vec3> vec3Scales;
};

class CAnimation : public CBaseAsset
{
public:
	DECLARE_CLASS( CAnimation, CBaseAsset );

	CAnimation( float flTime, const std::vector<SAnimationChannel *> &pAnimationChannels, const char *sPath );
	virtual ~CAnimation();

	virtual EAssetType GetAssetType( void ) const;

	float GetTime( void ) const;

	void GetTransform( glm::vec3 &vec3Position, glm::quat &qRotation, glm::vec3 &vec3Scale, unsigned int uiIndex, float flAnimationTime );

private:
	float m_flTime;
	std::vector<SAnimationChannel *> m_pAnimationChannels;
};

#endif // ANIMATION_H