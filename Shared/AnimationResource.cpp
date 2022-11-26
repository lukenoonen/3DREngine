#include "AnimationResource.h"
#include "ResourceManager.h"

CAnimationResource::CAnimationResource( float flTime, const std::vector<SAnimationChannel *> &pAnimationChannels, const char *sName ) : BaseClass( sName )
{
	m_flTime = flTime;
	m_pAnimationChannels = pAnimationChannels;
}

CAnimationResource::~CAnimationResource()
{
	for (unsigned int i = 0; i < m_pAnimationChannels.size(); i++)
	{
		if (m_pAnimationChannels[i])
			delete m_pAnimationChannels[i];
	}
}

float CAnimationResource::GetTime( void ) const
{
	return m_flTime;
}

void CAnimationResource::GetTransform( glm::vec3 &vec3Position, glm::quat &qRotation, glm::vec3 &vec3Scale, unsigned int uiIndex, float flAnimationTime ) const
{
	SAnimationChannel *pAnimationChannel = m_pAnimationChannels[uiIndex]; // Replace with a map or figure out some other way to save this stuff
	if (!pAnimationChannel)
	{
		vec3Position = g_vec3Zero;
		qRotation = glm::quat( 1.0f, 0.0f, 0.0f, 0.0f );
		vec3Scale = g_vec3One;
	}
	else
	{
		if (pAnimationChannel->vec3Positions.size() == 1)
		{
			vec3Position = pAnimationChannel->vec3Positions[0];
		}
		else
		{
			unsigned int uiPositionIndex = 0;
			for (unsigned int i = 0; i < pAnimationChannel->vec3Positions.size() - 1; i++)
			{
				if (flAnimationTime < pAnimationChannel->flPositionTimes[i + 1])
				{
					uiPositionIndex = i;
					break;
				}
			}

			unsigned int uiNextPositionIndex = uiPositionIndex + 1;

			float flFactor = (flAnimationTime - pAnimationChannel->flPositionTimes[uiPositionIndex]) / (pAnimationChannel->flPositionTimes[uiNextPositionIndex] - pAnimationChannel->flPositionTimes[uiPositionIndex]);

			const glm::vec3 &vec3Start = pAnimationChannel->vec3Positions[uiPositionIndex];
			const glm::vec3 &vec3End = pAnimationChannel->vec3Positions[uiNextPositionIndex];

			vec3Position = vec3Start + flFactor * (vec3End - vec3Start);
		}

		if (pAnimationChannel->qRotations.size() == 1)
		{
			qRotation = pAnimationChannel->qRotations[0];
		}
		else
		{
			unsigned int uiRotationIndex = 0;
			for (unsigned int i = 0; i < pAnimationChannel->flRotationTimes.size() - 1; i++)
			{
				if (flAnimationTime < pAnimationChannel->flRotationTimes[i + 1])
				{
					uiRotationIndex = i;
					break;
				}
			}

			unsigned int uiNextRotationIndex = uiRotationIndex + 1;

			float flFactor = (flAnimationTime - pAnimationChannel->flRotationTimes[uiRotationIndex]) / (pAnimationChannel->flRotationTimes[uiNextRotationIndex] - pAnimationChannel->flRotationTimes[uiRotationIndex]);

			const glm::quat &qStart = pAnimationChannel->qRotations[uiRotationIndex];
			const glm::quat &qEnd = pAnimationChannel->qRotations[uiNextRotationIndex];

			qRotation = glm::normalize( glm::slerp( qStart, qEnd, flFactor ) );
		}

		if (pAnimationChannel->vec3Scales.size() == 1)
		{
			vec3Scale = pAnimationChannel->vec3Scales[0];
		}
		else
		{
			unsigned int uiScaleIndex = 0;
			for (unsigned int i = 0; i < pAnimationChannel->flScaleTimes.size() - 1; i++)
			{
				if (flAnimationTime < pAnimationChannel->flScaleTimes[i + 1])
				{
					uiScaleIndex = i;
					break;
				}
			}

			unsigned int uiNextScaleIndex = uiScaleIndex + 1;

			float flFactor = (flAnimationTime - pAnimationChannel->flScaleTimes[uiScaleIndex]) / (pAnimationChannel->flScaleTimes[uiNextScaleIndex] - pAnimationChannel->flScaleTimes[uiScaleIndex]);

			const glm::vec3 &vec3Start = pAnimationChannel->vec3Scales[uiScaleIndex];
			const glm::vec3 &vec3End = pAnimationChannel->vec3Scales[uiNextScaleIndex];

			vec3Scale = vec3Start + flFactor * (vec3End - vec3Start);
		}
	}
}

bool UTIL_Write( CFile *pFile, CAnimationResource *&pData )
{
	const char *sName = pData->GetName();
	return UTIL_Write( pFile, sName );
}

bool UTIL_Read( CFile *pFile, CAnimationResource *&pData )
{
	char *sName;
	if (!UTIL_Read( pFile, sName ))
		return false;

	pData = pResourceManager->GetAnimationResource( sName );
	delete[] sName;
	return pData != NULL;
}

bool UTIL_GetValue( const CTextItem *pTextItem, CAnimationResource *&pValue )
{
	const char *sName;
	if (!UTIL_GetValue( pTextItem, sName ))
		return false;

	pValue = pResourceManager->GetAnimationResource( sName );
	return pValue != NULL;
}