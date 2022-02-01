#include "Animation.h"

CAnimationChannel::CAnimationChannel( const std::vector<float> &flPositionTimes, const std::vector<glm::vec3> &vec3Positions, const std::vector<float> &flRotationTimes, const std::vector<glm::quat> &qRotations, const std::vector<float> &flScaleTimes, const std::vector<glm::vec3> &vec3Scales )
{
	m_flPositionTimes = flPositionTimes;
	m_vec3Positions = vec3Positions;

	m_flRotationTimes = flRotationTimes;
	m_qRotations = qRotations;

	m_flScaleTimes = flScaleTimes;
	m_vec3Scales = vec3Scales;
}

void CAnimationChannel::CalcInterpolatedPosition( glm::vec3 &vec3Out, float flAnimationTime )
{
	if (m_vec3Positions.size() == 1)
	{
		vec3Out = m_vec3Positions[0];
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

	const glm::vec3 &vec3Start = m_vec3Positions[uiPositionIndex];
	const glm::vec3 &vec3End = m_vec3Positions[uiNextPositionIndex];

	vec3Out = vec3Start + flFactor * (vec3End - vec3Start);
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

void CAnimationChannel::CalcInterpolatedScale( glm::vec3 &vec3Out, float flAnimationTime )
{
	if (m_vec3Scales.size() == 1)
	{
		vec3Out = m_vec3Scales[0];
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

	const glm::vec3 &vec3Start = m_vec3Scales[uiScaleIndex];
	const glm::vec3 &vec3End = m_vec3Scales[uiNextScaleIndex];

	vec3Out = vec3Start + flFactor * (vec3End - vec3Start);
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