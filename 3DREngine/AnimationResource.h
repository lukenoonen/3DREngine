#ifndef ANIMATIONRESOURCE_H
#define ANIMATIONRESOURCE_H

#include "Global.h"
#include "BaseResource.h"
#include "AnimationResourceData.h"

class CAnimationResource : public CBaseResource
{
public:
	DECLARE_CLASS( CAnimationResource, CBaseResource )

	CAnimationResource( SAnimationResourceData &sData, const char *sName );
	virtual ~CAnimationResource();

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