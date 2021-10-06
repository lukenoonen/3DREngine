#include "Animation.h"

CAnimationChannel::CAnimationChannel( const std::vector<float> &flPositionTimes, const std::vector<glm::vec3> &vecPositions, const std::vector<float> &flRotationTimes, const std::vector<glm::quat> &qRotations, const std::vector<float> &flScaleTimes, const std::vector<glm::vec3> &vecScales )
{
	m_flPositionTimes = flPositionTimes;
	m_vecPositions = vecPositions;

	m_flRotationTimes = flRotationTimes;
	m_qRotations = qRotations;

	m_flScaleTimes = flScaleTimes;
	m_vecScales = vecScales;
}

void CAnimationChannel::CalcInterpolatedPosition( glm::vec3 &vecOut, float flAnimationTime )
{
	if (m_vecPositions.size() == 1)
	{
		vecOut = m_vecPositions[0];
		return;
	}

	unsigned int uiPositionIndex = 0;
	for (unsigned int i = 0; i < m_flPositionTimes.size() - 1; i++)
	{
		if (flAnimationTime < m_flPositionTimes[i + 1])
		{
			uiPositionIndex = i;
			break;
		}
	}

	unsigned int uiNextPositionIndex = uiPositionIndex + 1;

	float flFactor = (flAnimationTime - m_flPositionTimes[uiPositionIndex]) / (m_flPositionTimes[uiNextPositionIndex] - m_flPositionTimes[uiPositionIndex]);

	const glm::vec3 &vecStart = m_vecPositions[uiPositionIndex];
	const glm::vec3 &vecEnd = m_vecPositions[uiNextPositionIndex];

	vecOut = vecStart + flFactor * (vecEnd - vecStart);
}

void CAnimationChannel::CalcInterpolatedRotation( glm::quat &qOut, float flAnimationTime )
{
	if (m_qRotations.size() == 1)
	{
		qOut = m_qRotations[0];
		return;
	}

	unsigned int uiRotationIndex = 0;
	for (unsigned int i = 0; i < m_flRotationTimes.size() - 1; i++)
	{
		if (flAnimationTime < m_flRotationTimes[i + 1])
		{
			uiRotationIndex = i;
			break;
		}
	}

	unsigned int uiNextRotationIndex = uiRotationIndex + 1;

	float flFactor = (flAnimationTime - m_flRotationTimes[uiRotationIndex]) / (m_flRotationTimes[uiNextRotationIndex] - m_flRotationTimes[uiRotationIndex]);

	const glm::quat &qStart = m_qRotations[uiRotationIndex];
	const glm::quat &qEnd = m_qRotations[uiNextRotationIndex];

	qOut = glm::normalize( glm::slerp( qStart, qEnd, flFactor ) );
}

void CAnimationChannel::CalcInterpolatedScale( glm::vec3 &vecOut, float flAnimationTime )
{
	if (m_vecScales.size() == 1)
	{
		vecOut = m_vecScales[0];
		return;
	}

	unsigned int uiScaleIndex = 0;
	for (unsigned int i = 0; i < m_flScaleTimes.size() - 1; i++)
	{
		if (flAnimationTime < m_flScaleTimes[i + 1])
		{
			uiScaleIndex = i;
			break;
		}
	}

	unsigned int uiNextScaleIndex = uiScaleIndex + 1;

	float flFactor = (flAnimationTime - m_flScaleTimes[uiScaleIndex]) / (m_flScaleTimes[uiNextScaleIndex] - m_flScaleTimes[uiScaleIndex]);

	const glm::vec3 &vecStart = m_vecScales[uiScaleIndex];
	const glm::vec3 &vecEnd = m_vecScales[uiNextScaleIndex];

	vecOut = vecStart + flFactor * (vecEnd - vecStart);
}

CAnimation::CAnimation( float flTime, const std::vector<CAnimationChannel *> &pAnimationChannels, const char *sPath ) : BaseClass( sPath )
{
	m_flTime = flTime;
	m_pAnimationChannels = pAnimationChannels;
}

CAnimation::~CAnimation()
{
	for (unsigned int i = 0; i < (unsigned int)m_pAnimationChannels.size(); i++)
	{
		if (m_pAnimationChannels[i])
			delete m_pAnimationChannels[i];
	}
}

float CAnimation::GetTime( void ) const
{
	return m_flTime;
}

CAnimationChannel *CAnimation::GetAnimationChannel( unsigned int uiIndex ) const
{
	return m_pAnimationChannels[uiIndex];
}