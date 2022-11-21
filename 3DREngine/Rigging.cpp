#include "Rigging.h"

DEFINE_DATADESC( CRigging )

	DEFINE_FIELD( LinkedDataField, CHandle<CSkeleton>, m_hSkeleton, "skeleton", FL_REQUIRED )
	DEFINE_FIELD( LinkedVectorDataField, CHandle<CAnimation>, m_hAnimations, "animations", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CRigging, rigging )

CRigging::CRigging()
{

}

unsigned int CRigging::GetBonesCount( void )
{
	return m_hSkeleton->GetBonesCount();
}

float CRigging::GetAnimationTime( unsigned int uiAnimationIndex )
{
	return m_hAnimations[uiAnimationIndex]->GetTime();
}

void CRigging::UpdateAnimation( std::vector<glm::mat4> &matBoneTransforms, const std::vector<unsigned int> &uiAnimations, const std::vector<float> &flAnimationTimes, const std::vector<float> &flAnimationTransitionFactors )
{
	unsigned int uiAnimationCount = (unsigned int)uiAnimations.size();
	std::vector<CAnimation *> pAnimations;
	pAnimations.resize( uiAnimationCount );
	for (unsigned int i = 0; i < uiAnimationCount; i++)
		pAnimations[i] = m_hAnimations[uiAnimations[i]];

	m_hSkeleton->UpdateAnimation( matBoneTransforms, pAnimations, flAnimationTimes, flAnimationTransitionFactors );
}