#include "Rigging.h"

CRigging::CRigging()
{
	m_pSkeleton = NULL;
}

bool CRigging::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (!m_pSkeleton)
		return false;

	if (!m_pAnimations.empty())
		return false;

	return true;
}

unsigned int CRigging::GetBonesCount( void )
{
	return m_pSkeleton->GetBonesCount();
}

float CRigging::GetAnimationTime( unsigned int uiAnimationIndex )
{
	return m_pAnimations[uiAnimationIndex]->GetTime();
}

void CRigging::UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<unsigned int> &uiAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors )
{
	unsigned int uiAnimationCount = uiAnimations.size();
	std::vector<CAnimation *> pAnimations;
	pAnimations.resize( uiAnimationCount );
	for (unsigned int i = 0; i < uiAnimationCount; i++)
		pAnimations[i] = m_pAnimations[uiAnimations[i]];

	m_pSkeleton->UpdateAnimation( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors );
}

void CRigging::SetSkeleton( CSkeleton *pSkeleton )
{
	m_pSkeleton = pSkeleton;
}

void CRigging::AddAnimation( CAnimation *pAnimation )
{
	m_pAnimations.push_back( pAnimation );
}