#ifndef ANIMATION_H
#define ANIMATION_H

#include "Global.h"
#include "BaseAsset.h"

class CAnimationChannel
{
public:
	DECLARE_CLASS_NOBASE( CAnimationChannel );

	CAnimationChannel( const std::vector<float> &flPositionTimes, const std::vector<glm::vec3> &vecPositions, const std::vector<float> &flRotationTimes, const std::vector<glm::quat> &qRotations, const std::vector<float> &flScaleTimes, const std::vector<glm::vec3> &vecScales );

	void CalcInterpolatedPosition( glm::vec3 &vecOut, float flAnimationTime );
	void CalcInterpolatedRotation( glm::quat &qOut, float flAnimationTime );
	void CalcInterpolatedScale( glm::vec3 &vecOut, float flAnimationTime );

private:
	std::vector<float> m_flPositionTimes;
	std::vector<glm::vec3> m_vecPositions;

	std::vector<float> m_flRotationTimes;
	std::vector<glm::quat> m_qRotations;

	std::vector<float> m_flScaleTimes;
	std::vector<glm::vec3> m_vecScales;
};

class CAnimation : public CBaseAsset
{
public:
	DECLARE_CLASS( CAnimation, CBaseAsset );

	CAnimation( float flTime, const std::vector<CAnimationChannel *> &pAnimationChannels, const char *sPath );
	virtual ~CAnimation();

	float GetTime( void ) const;
	CAnimationChannel *GetAnimationChannel( unsigned int uiIndex ) const;

private:
	float m_flTime;
	std::vector<CAnimationChannel *> m_pAnimationChannels;
};

#endif // ANIMATION_H