#include "AnimationResourceData.h"

#ifdef PROJECT_ENGINE
bool UTIL_ReadInternal( CFile *pFile, SAnimationResourceData &dData )
{
	if (!pFile->Read( dData.flTime ))
		return false;

	unsigned int uiAnimationChannelSlots;
	if (!pFile->Read( uiAnimationChannelSlots ))
		return false;

	dData.pAnimationChannels.resize( uiAnimationChannelSlots, NULL );

	for (unsigned int i = 0; i < uiAnimationChannelSlots; i++)
	{
		bool bSlotFilled;
		if (!pFile->Read( bSlotFilled ))
			return false;

		if (bSlotFilled)
		{
			SAnimationChannel *pAnimationChannel = new SAnimationChannel;
			dData.pAnimationChannels[i] = pAnimationChannel;

			unsigned int uiPositionsCount;
			if (!pFile->Read( uiPositionsCount ))
				return false;

			pAnimationChannel->flPositionTimes.resize( uiPositionsCount );
			pAnimationChannel->vec3Positions.resize( uiPositionsCount );

			for (unsigned int j = 0; j < uiPositionsCount; j++)
			{
				if (!pFile->Read( pAnimationChannel->flPositionTimes[j] ) || !pFile->Read( pAnimationChannel->vec3Positions[j] ))
					return false;
			}

			unsigned int uiRotationsCount;
			if (!pFile->Read( uiRotationsCount ))
				return false;

			pAnimationChannel->flRotationTimes.resize( uiRotationsCount );
			pAnimationChannel->qRotations.resize( uiRotationsCount );

			for (unsigned int j = 0; j < uiRotationsCount; j++)
			{
				if (!pFile->Read( pAnimationChannel->flRotationTimes[j] ) || !pFile->Read( pAnimationChannel->qRotations[j].w ) || !pFile->Read( pAnimationChannel->qRotations[j].x ) || !pFile->Read( pAnimationChannel->qRotations[j].y ) || !pFile->Read( pAnimationChannel->qRotations[j].z ))
					return false;
			}

			unsigned int uiScalesCount;
			if (!pFile->Read( uiScalesCount ))
				return false;

			pAnimationChannel->flScaleTimes.resize( uiScalesCount );
			pAnimationChannel->vec3Scales.resize( uiScalesCount );

			for (unsigned int j = 0; j < uiScalesCount; j++)
			{
				if (!pFile->Read( pAnimationChannel->flScaleTimes[j] ) || !pFile->Read( pAnimationChannel->vec3Scales[j] ))
					return false;
			}
		}
	}

	return true;
}

bool UTIL_Read( CFile *pFile, SAnimationResourceData &dData )
{
	bool bResult = UTIL_ReadInternal( pFile, dData );

	if (bResult)
	{
		for (unsigned int i = 0; i < dData.pAnimationChannels.size(); i++)
			delete dData.pAnimationChannels[i];
	}

	return bResult;
}
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, SAnimationResourceData &dData )
{
	if (!pFile->Write( dData.flTime ))
		return false;

	unsigned int uiAnimationChannelSlots = (unsigned int)dData.pAnimationChannels.size();
	if (!pFile->Write( uiAnimationChannelSlots ))
		return false;

	for (unsigned int i = 0; i < uiAnimationChannelSlots; i++)
	{
		SAnimationChannel *pAnimationChannel = dData.pAnimationChannels[i];
		bool bSlotFilled = pAnimationChannel != NULL;
		if (!pFile->Write( bSlotFilled ))
			return false;

		if (bSlotFilled)
		{
			unsigned int uiPositionsCount = (unsigned int)pAnimationChannel->vec3Positions.size();
			if (!pFile->Write( uiPositionsCount ))
				return false;

			for (unsigned int j = 0; j < uiPositionsCount; j++)
			{
				if (!pFile->Write( pAnimationChannel->flPositionTimes[j] ) || !pFile->Write( pAnimationChannel->vec3Positions[j] ))
					return false;
			}

			unsigned int uiRotationsCount = (unsigned int)pAnimationChannel->qRotations.size();
			if (!pFile->Write( uiRotationsCount ))
				return false;

			for (unsigned int j = 0; j < uiRotationsCount; j++)
			{
				if (!pFile->Write( pAnimationChannel->flRotationTimes[j] ) || !pFile->Write( pAnimationChannel->qRotations[j].w ) || !pFile->Write( pAnimationChannel->qRotations[j].x ) || !pFile->Write( pAnimationChannel->qRotations[j].y ) || !pFile->Write( pAnimationChannel->qRotations[j].z ))
					return false;
			}

			unsigned int uiScalesCount = (unsigned int)pAnimationChannel->vec3Scales.size();
			if (!pFile->Write( uiScalesCount ))
				return false;

			for (unsigned int j = 0; j < uiScalesCount; j++)
			{
				if (!pFile->Write( pAnimationChannel->flScaleTimes[j] ) || !pFile->Write( pAnimationChannel->vec3Scales[j] ))
					return false;
			}
		}
	}

	return true;
}
#endif // PROJECT_RESOURCE