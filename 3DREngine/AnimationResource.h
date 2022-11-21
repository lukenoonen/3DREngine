#ifndef ANIMATIONRESOURCE_H
#define ANIMATIONRESOURCE_H

#include "Global.h"
#include "BaseResource.h"

struct SAnimationChannel
{
	std::vector<float> flPositionTimes;
	std::vector<glm::vec3> vec3Positions;

	std::vector<float> flRotationTimes;
	std::vector<glm::quat> qRotations;

	std::vector<float> flScaleTimes;
	std::vector<glm::vec3> vec3Scales;
};

class CAnimationResource : public CBaseResource
{
public:
	DECLARE_CLASS( CAnimationResource, CBaseResource )

	CAnimationResource( float flTime, const std::vector<SAnimationChannel *> &pAnimationChannels, const char *sName );
	~CAnimationResource();

	float GetTime( void ) const;

	void GetTransform( glm::vec3 &vec3Position, glm::quat &qRotation, glm::vec3 &vec3Scale, unsigned int uiIndex, float flAnimationTime ) const;

private:
	float m_flTime;
	std::vector<SAnimationChannel *> m_pAnimationChannels;
};

bool UTIL_Write( CFile *pFile, CAnimationResource *&pData );
bool UTIL_Read( CFile *pFile, CAnimationResource *&pData );
bool UTIL_GetValue( const CTextItem *pTextItem, CAnimationResource *&pValue );

#endif // ANIMATIONRESOURCE_H