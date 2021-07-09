#ifndef ANIMATION_H
#define ANIMATION_H

#include "Global.h"

struct SAnimationChannel
{
	unsigned int uiAssociatedNode;

	std::vector<float> flPositionTimes;
	std::vector<glm::vec3> vecPositions;

	std::vector<float> flRotationTimes;
	std::vector<glm::quat> qRotations;

	std::vector<float> flScaleTimes;
	std::vector<glm::vec3> vecScales;
};

class CAnimation
{
public:
	DECLARE_CLASS_NOBASE( CAnimation );

	CAnimation( const char *sPath, float flDuration, float flTicksPerSecond, const std::vector<SAnimationChannel> &acAnimationChannels );
	~CAnimation();

	float GetDuration( void );
	float GetTicksPerSecond( void );

	unsigned int GetAnimationChannelCount( void );
	const SAnimationChannel &GetAnimationChannel( unsigned int uiIndex );

	const char *GetPath( void );

	void CalcInterpolatedTranslation( glm::vec3 &vecOut, float flAnimationTime, unsigned int uiIndex );
	void CalcInterpolatedRotation( glm::quat &qOut, float flAnimationTime, unsigned int uiIndex );
	void CalcInterpolatedScaling( glm::vec3 &vecOut, float flAnimationTime, unsigned int uiIndex );
	
	unsigned int FindTranslation( float flAnimationTime, unsigned int uiIndex );
	unsigned int FindRotation( float flAnimationTime, unsigned int uiIndex );
	unsigned int FindScale( float flAnimationTime, unsigned int uiIndex );

private:
	float m_flDuration;
	float m_flTicksPerSecond;

	std::vector<SAnimationChannel> m_acAnimationChannels;
	
	char *m_sPath;
};

#endif // ANIMATION_H