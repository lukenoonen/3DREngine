#ifndef ANIMATIONRESOURCEDATA_H
#define ANIMATIONRESOURCEDATA_H

#include "Global.h"

struct SAnimationChannel
{
	std::vector<float> flPositionTimes;
	std::vector<glm::vec3> vec3Positions;

	std::vector<float> flRotationTimes;
	std::vector<glm::quat> qRotations;

	std::vector<float> flScaleTimes;
	std::vector<glm::vec3> vec3Scales;
};

struct SAnimationResourceData
{
	float flTime;
	std::vector<SAnimationChannel *> pAnimationChannels;
};

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, SAnimationResourceData &dData );
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, SAnimationResourceData &dData );
#endif // PROJECT_RESOURCE

#endif // ANIMATIONRESOURCEDATA_H