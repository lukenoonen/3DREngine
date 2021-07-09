#include "Animation.h"
#include <UTIL.h>

CAnimation::CAnimation( const char *sPath, float flDuration, float flTicksPerSecond, const std::vector<SAnimationChannel> &acAnimationChannels )
{
	m_sPath = UTIL_stredit( sPath );

	m_flDuration = flDuration;
	m_flTicksPerSecond = flTicksPerSecond;

	m_acAnimationChannels = acAnimationChannels;
}

CAnimation::~CAnimation()
{
	delete[] m_sPath;
}

float CAnimation::GetDuration( void )
{
	return m_flDuration;
}

float CAnimation::GetTicksPerSecond( void )
{
	return m_flTicksPerSecond;
}

unsigned int CAnimation::GetAnimationChannelCount( void )
{
	return (unsigned int)m_acAnimationChannels.size();
}

const SAnimationChannel &CAnimation::GetAnimationChannel( unsigned int uiIndex )
{
	return m_acAnimationChannels[uiIndex];
}

const char *CAnimation::GetPath(void)
{
	return m_sPath;
}

void CAnimation::CalcInterpolatedTranslation( glm::vec3 &vecOut, float flAnimationTime, unsigned int uiIndex )
{
	if (m_acAnimationChannels[uiIndex].vecPositions.size() == 1)
	{
		vecOut = m_acAnimationChannels[uiIndex].vecPositions[0];
		return;
	}

	unsigned int uiPositionIndex = FindTranslation( flAnimationTime, uiIndex );
	unsigned int uiNextPositionIndex = (uiPositionIndex + 1);
	float flPositionIndexTime = m_acAnimationChannels[uiIndex].flPositionTimes[uiPositionIndex];
	float flFactor = (flAnimationTime - flPositionIndexTime) / (m_acAnimationChannels[uiIndex].flPositionTimes[uiNextPositionIndex] - flPositionIndexTime);
	
	const glm::vec3& vecStart = m_acAnimationChannels[uiIndex].vecPositions[uiPositionIndex];
	const glm::vec3& vecEnd = m_acAnimationChannels[uiIndex].vecPositions[uiNextPositionIndex];
	
	vecOut = vecStart + flFactor * (vecEnd - vecStart);
}

void CAnimation::CalcInterpolatedRotation( glm::quat &qOut, float flAnimationTime, unsigned int uiIndex )
{
	if (m_acAnimationChannels[uiIndex].qRotations.size() == 1)
	{
		qOut = m_acAnimationChannels[uiIndex].qRotations[0];
		return;
	}

	unsigned int uiRotationIndex = FindRotation( flAnimationTime, uiIndex );
	unsigned int uiNextRotationIndex = (uiRotationIndex + 1);
	float flRotationIndexTime = m_acAnimationChannels[uiIndex].flRotationTimes[uiRotationIndex];
	float flFactor = (flAnimationTime - flRotationIndexTime) / (m_acAnimationChannels[uiIndex].flRotationTimes[uiNextRotationIndex] - flRotationIndexTime);
	
	const glm::quat& qStart = m_acAnimationChannels[uiIndex].qRotations[uiRotationIndex];
	const glm::quat& qEnd = m_acAnimationChannels[uiIndex].qRotations[uiNextRotationIndex];
	
	qOut = glm::slerp( qStart, qEnd, flFactor );
	qOut = glm::normalize( qOut );
}

void CAnimation::CalcInterpolatedScaling( glm::vec3 &vecOut, float flAnimationTime, unsigned int uiIndex )
{
	if (m_acAnimationChannels[uiIndex].vecScales.size() == 1)
	{
		vecOut = m_acAnimationChannels[uiIndex].vecScales[0];
		return;
	}

	unsigned int uiScaleIndex = FindScale( flAnimationTime, uiIndex );
	unsigned int uiNextScaleIndex = (uiScaleIndex + 1);
	float flScaleIndexTime = m_acAnimationChannels[uiIndex].flScaleTimes[uiScaleIndex];
	float flFactor = (flAnimationTime - flScaleIndexTime) / (m_acAnimationChannels[uiIndex].flScaleTimes[uiNextScaleIndex] - flScaleIndexTime);
	
	const glm::vec3& vecStart = m_acAnimationChannels[uiIndex].vecScales[uiScaleIndex];
	const glm::vec3& vecEnd = m_acAnimationChannels[uiIndex].vecScales[uiNextScaleIndex];
	
	vecOut = vecStart + flFactor * (vecEnd - vecStart);
}

unsigned int CAnimation::FindTranslation( float flAnimationTime, unsigned int uiIndex )
{
	for (unsigned int i = 0; i < m_acAnimationChannels[uiIndex].flPositionTimes.size() - 1; i++)
	{
		if (flAnimationTime < m_acAnimationChannels[uiIndex].flPositionTimes[i + 1])
			return i;
	}

	return 0;
}

unsigned int CAnimation::FindRotation( float flAnimationTime, unsigned int uiIndex )
{
	for (unsigned int i = 0; i < m_acAnimationChannels[uiIndex].flRotationTimes.size() - 1; i++)
	{
		if (flAnimationTime < m_acAnimationChannels[uiIndex].flRotationTimes[i + 1])
			return i;
	}

	return 0;
}

unsigned int CAnimation::FindScale( float flAnimationTime, unsigned int uiIndex )
{
	for (unsigned int i = 0; i < m_acAnimationChannels[uiIndex].flScaleTimes.size() - 1; i++)
	{
		if (flAnimationTime < m_acAnimationChannels[uiIndex].flScaleTimes[i + 1])
			return i;
	}

	return 0;
}