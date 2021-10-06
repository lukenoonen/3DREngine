#include "AnimationManager.h"
#include <UTIL.h>
#include <fstream>

CAnimationManager::CAnimationManager()
{

}

CAnimationManager::~CAnimationManager()
{
	Reset();
}

void CAnimationManager::Reset( void )
{
	for (unsigned int i = 0; i < m_pAnimations.size(); i++)
		delete m_pAnimations[i];

	for (unsigned int i = 0; i < m_pAnimationControllers.size(); i++)
		delete m_pAnimationControllers[i];

	m_pAnimations.clear();

	m_pAnimationControllers.clear();
}

CAnimationController *CAnimationManager::GetAnimationController( const char *sPath )
{
	char sFullPath[260];
	sprintf_s( sFullPath, sizeof( sFullPath ), "resources/animations/%s", sPath );

	std::fstream fAnimationController( sFullPath, std::ios::in | std::ios::binary );
	if (!fAnimationController.is_open())
		ExitError( "Cannot open animation controller file." );

	std::vector<CAnimation *> pAnimations;
	std::vector<unsigned int> uiNextAnimations;

	unsigned int uiAnimationsCount;
	UTIL_Read( fAnimationController, &uiAnimationsCount, 1, unsigned int );

	for (unsigned int i = 0; i < uiAnimationsCount; i++)
	{
		char *sAnimationName;
		UTIL_ReadString( fAnimationController, sAnimationName );
		pAnimations.push_back( GetAnimation( sAnimationName ) );
		delete[] sAnimationName;

		unsigned int uiNextAnimation;
		UTIL_Read( fAnimationController, &uiNextAnimation, 1, unsigned int );
		uiNextAnimations.push_back( uiNextAnimation );
	}

	CAnimationController *pNewAnimationController = new CAnimationController( pAnimations, uiNextAnimations );
	m_pAnimationControllers.push_back( pNewAnimationController );
	return pNewAnimationController;
}

CAnimation *CAnimationManager::GetAnimation( const char *sPath )
{
	for (unsigned int i = 0; i < m_pAnimations.size(); i++)
	{
		if (UTIL_strcmp( m_pAnimations[i]->GetPath(), sPath ) == 0)
			return m_pAnimations[i];
	}

	CAnimation *pNewAnimation = CreateAnimation( sPath );
	m_pAnimations.push_back( pNewAnimation );
	return pNewAnimation;
}

CAnimation *CAnimationManager::CreateAnimation( const char *sPath )
{
	char sFullPath[260];
	sprintf_s( sFullPath, sizeof(sFullPath), "resources/animations/%s", sPath );

	std::fstream fAnimation( sFullPath, std::ios::in | std::ios::binary );
	if (!fAnimation.is_open())
		ExitError( "Cannot open animation file." );
	
	float flDuration;
	float flTicksPerSecond;
	std::vector<SAnimationChannel> acAnimationChannels;

	UTIL_Read( fAnimation, &flDuration, 1, float );
	UTIL_Read( fAnimation, &flTicksPerSecond, 1, float );

	unsigned int uiAssociatedNode;
	while (UTIL_Read( fAnimation, &uiAssociatedNode, 1, unsigned int ))
	{
		SAnimationChannel acAnimationChannel;
		acAnimationChannel.uiAssociatedNode = uiAssociatedNode;

		unsigned int uiNumPositions;
		UTIL_Read( fAnimation, &uiNumPositions, 1, unsigned int );
		for (unsigned int i = 0; i < uiNumPositions; i++)
		{
			float flTime;
			glm::vec3 vecPosition;
			UTIL_Read( fAnimation, &flTime, 1, float );
			UTIL_Read( fAnimation, &vecPosition, 1, glm::vec3 );
			acAnimationChannel.flPositionTimes.push_back( flTime );
			acAnimationChannel.vecPositions.push_back( vecPosition );
		}

		unsigned int uiNumRotations;
		UTIL_Read( fAnimation, &uiNumRotations, 1, unsigned int );
		for (unsigned int i = 0; i < uiNumRotations; i++)
		{
			float flTime;
			glm::quat qRotation;
			UTIL_Read( fAnimation, &flTime, 1, float );
			UTIL_Read( fAnimation, &qRotation.w, 1, float );
			UTIL_Read( fAnimation, &qRotation.x, 1, float );
			UTIL_Read( fAnimation, &qRotation.y, 1, float );
			UTIL_Read( fAnimation, &qRotation.z, 1, float );
			acAnimationChannel.flRotationTimes.push_back( flTime );
			acAnimationChannel.qRotations.push_back( qRotation );
		}

		unsigned int uiNumScales;
		UTIL_Read( fAnimation, &uiNumScales, 1, unsigned int );
		for (unsigned int i = 0; i < uiNumScales; i++)
		{
			float flTime;
			glm::vec3 vecScale;
			UTIL_Read( fAnimation, &flTime, 1, float );
			UTIL_Read( fAnimation, &vecScale, 1, glm::vec3 );
			acAnimationChannel.flScaleTimes.push_back( flTime );
			acAnimationChannel.vecScales.push_back( vecScale );
		}

		acAnimationChannels.push_back( acAnimationChannel );
	}

	return new CAnimation( sPath, flDuration, flTicksPerSecond, acAnimationChannels );
}