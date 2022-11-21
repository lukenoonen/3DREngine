#include "BaseAnimated.h"
#include "RenderManager.h"
#include "TimeManager.h"

DEFINE_DATADESC_NOBASE( CBaseAnimated )

	DEFINE_FIELD( DataField, float, m_flAnimationTimeScale, "animationtimescale", 0 )

END_DATADESC()

CBaseAnimated::CBaseAnimated()
{
	m_flAnimationTimeScale = 1.0f;
}

bool CBaseAnimated::Init( void )
{
	if (!BaseClass::Init())
		return false;

	CModel *pModel = GetModel();

	if (!pModel->IsAnimated())
		return false;

	m_matBoneTransforms.resize( pModel->GetBonesCount() );

	return true;
}

void CBaseAnimated::PostThink( void )
{
	if (HasFlags( FL_ANIMATE | FL_UPDATEANIMATION ))
	{
		CModel *pModel = GetModel();

		for (unsigned int i = 0; i < m_flAnimationTransitionFactors.size(); i++)
		{
			m_flAnimationTransitionFactors[i] += pTimeManager->GetFrameTime() / m_flAnimationTransitionTimes[i];
			if (m_flAnimationTransitionFactors[i] >= 1.0f)
			{
				for (unsigned int j = 0; j <= i; j++)
				{
					m_uiAnimations.erase( m_uiAnimations.begin() );
					m_flAnimationTimes.erase( m_flAnimationTimes.begin() );
					m_flAnimationTransitionFactors.erase( m_flAnimationTransitionFactors.begin() );
					m_flAnimationTransitionTimes.erase( m_flAnimationTransitionTimes.begin() );
				}

				i = 0;
			}
		}

		for (unsigned int i = 0; i < m_uiAnimations.size(); i++)
			m_flAnimationTimes[i] = fmod( m_flAnimationTimes[i] + pTimeManager->GetFrameTime() * m_flAnimationTimeScale, pModel->GetAnimationTime( m_uiAnimations[i] ) );

		std::vector<float> flAdjustedAnimationTransitionFactors;
		for (unsigned int i = 0; i < m_flAnimationTransitionFactors.size(); i++)
		{
			float flAdjustedAnimationTransitionFactor = (m_flAnimationTransitionFactors[i] - 1.0f) / m_flAnimationTransitionFactors[i];
			flAdjustedAnimationTransitionFactor = 1.0f / (1.0f + flAdjustedAnimationTransitionFactor * flAdjustedAnimationTransitionFactor);
			flAdjustedAnimationTransitionFactors.push_back( flAdjustedAnimationTransitionFactor );
		}

		pModel->UpdateAnimation( m_matBoneTransforms, m_uiAnimations, m_flAnimationTimes, flAdjustedAnimationTransitionFactors );
	}

	BaseClass::PostThink();
}

void CBaseAnimated::PreDraw( void )
{
	if (HasFlags( FL_ANIMATE ))
	{
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_animate, (EBaseEnum)EShaderPreprocessorAnimate::t_true );
		pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_bones, 0, 0, UTIL_min( (int)m_matBoneTransforms.size(), 64 ), &m_matBoneTransforms[0] );
	}

	BaseClass::PreDraw();
}

void CBaseAnimated::PostDraw( void )
{
	if (HasFlags( FL_ANIMATE ))
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_animate, (EBaseEnum)EShaderPreprocessorAnimate::t_false );

	BaseClass::PostDraw();
}

void CBaseAnimated::TransitionAnimation( unsigned int uiAnimationIndex, float flAnimationTransitionTime )
{
	m_uiAnimations.push_back( uiAnimationIndex );
	m_flAnimationTimes.push_back( 0.0f );
	m_flAnimationTransitionFactors.push_back( 0.0f );
	m_flAnimationTransitionTimes.push_back( flAnimationTransitionTime );
}

void CBaseAnimated::SetAnimation( unsigned int uiAnimationIndex )
{
	m_uiAnimations.clear();
	m_flAnimationTimes.clear();
	m_flAnimationTransitionFactors.clear();
	m_flAnimationTransitionTimes.clear();

	m_uiAnimations.push_back( uiAnimationIndex );
	m_flAnimationTimes.push_back( 0.0f );
}