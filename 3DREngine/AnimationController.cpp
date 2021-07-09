#include "AnimationController.h"
#include "GlobalManager.h"

CAnimationController::CAnimationController( const std::vector<CAnimation *> &pAnimations, const std::vector<unsigned int> &uiNextAnimations )
{
	m_pAnimations = pAnimations;
	m_uiNextAnimations = uiNextAnimations;

	m_uiCurrentAnimation = 0;
	m_flCurrentAnimationTime = 0.0f;

	m_bAnimationQueued = false;

	m_flAnimationRate = 1.0f;
}

bool CAnimationController::ShouldAnimate( void )
{
	return m_matBoneTransform.size() > 0;
}

void CAnimationController::PlayAnimation( unsigned int uiIndex )
{
	m_uiCurrentAnimation = uiIndex;
	m_flCurrentAnimationTime = 0.0f;
}

void CAnimationController::QueueAnimation( unsigned int uiIndex )
{
	m_bAnimationQueued = true;
	m_uiQueuedAnimation = uiIndex;
}

void CAnimationController::UpdateAnimation( void )
{
	m_flCurrentAnimationTime += pGlobalValues->GetFrameTime() * m_flAnimationRate;
	while (GetCurrentTime() >= GetCurrentAnimation()->GetDuration())
	{
		m_flCurrentAnimationTime -= GetCurrentAnimation()->GetDuration() / GetCurrentAnimation()->GetTicksPerSecond();
		if (m_bAnimationQueued)
		{
			m_bAnimationQueued = false;
			m_uiCurrentAnimation = m_uiQueuedAnimation;
		}
		else
		{
			m_uiCurrentAnimation = m_uiNextAnimations[m_uiCurrentAnimation];
		}
	}
}

CAnimation *CAnimationController::GetCurrentAnimation( void )
{
	return m_pAnimations[m_uiCurrentAnimation];
}

float CAnimationController::GetCurrentTime( void )
{
	return m_flCurrentAnimationTime * GetCurrentAnimation()->GetTicksPerSecond();
}

void CAnimationController::SetAnimationRate( float flAnimationRate )
{
	m_flAnimationRate = flAnimationRate;
}

void CAnimationController::SetBoneTransformSize( unsigned int uiSize )
{
	m_matBoneTransform.resize( uiSize );
}

void CAnimationController::SetBoneTransformation( unsigned int uiIndex, const glm::mat4 &matTransform )
{
	m_matBoneTransform[uiIndex] = matTransform;
}

const std::vector<glm::mat4> &CAnimationController::GetBoneTransforms( void )
{
	return m_matBoneTransform;
}