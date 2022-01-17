#include "BaseAnimated.h"
#include "RenderManager.h"
#include "GlobalManager.h"

CBaseAnimated::CBaseAnimated()
{
	m_bAnimate = false;
	m_bUpdateAnimation = false;

	m_flAnimationTimeScale = 1.0f;
}

void CBaseAnimated::PostThink( void )
{
	if (m_bAnimate && m_bUpdateAnimation)
	{
		CModel *pModel = GetModel();
		if (pModel->IsAnimated())
		{
			for (unsigned int i = 0; i < (unsigned int)m_flAnimationTransitionFactors.size(); i++)
			{
				m_flAnimationTransitionFactors[i] += pGlobalValues->GetFrameTime() / m_flAnimationTransitionTimes[i];
				if (m_flAnimationTransitionFactors[i] >= 1.0f)
				{
					for (unsigned int j = 0; j <= i; j++)
					{
						m_pAnimations.erase( m_pAnimations.begin() );
						m_flAnimationTimes.erase( m_flAnimationTimes.begin() );
						m_flAnimationTransitionFactors.erase( m_flAnimationTransitionFactors.begin() );
						m_flAnimationTransitionTimes.erase( m_flAnimationTransitionTimes.begin() );
					}

					i = 0;
				}
			}

			for (unsigned int i = 0; i < (unsigned int)m_pAnimations.size(); i++)
				m_flAnimationTimes[i] = fmod( m_flAnimationTimes[i] + pGlobalValues->GetFrameTime() * m_flAnimationTimeScale, m_pAnimations[i]->GetTime() );

			pModel->UpdateAnimation( m_matBoneTransforms, m_pAnimations, m_flAnimationTimes, m_flAnimationTransitionFactors );
		}	
	}

	BaseClass::PostThink();
}

void CBaseAnimated::PreDraw( void )
{
	BaseClass::PreDraw();

	pShaderManager->SetShaderAnimate( SHADERANIMATE_TRUE );

	if (m_bAnimate)
	{
		CModel *pModel = GetModel();
		if (pModel->IsAnimated())
			pShaderManager->SetUniformBufferObject( UBO_BONES, 0, 0, (unsigned int)UTIL_Min( (unsigned int)m_matBoneTransforms.size(), 64 ), &m_matBoneTransforms[0] );
	}
}

void CBaseAnimated::PostDraw( void )
{
	pShaderManager->SetShaderAnimate( SHADERANIMATE_FALSE );
}

void CBaseAnimated::SetModel( CModel *pModel )
{
	BaseClass::SetModel( pModel );
	pModel->SetUpBoneTransforms( m_matBoneTransforms );
}

void CBaseAnimated::SetAnimate( bool bAnimate )
{
	m_bAnimate = bAnimate;
}

void CBaseAnimated::SetUpdateAnimation( bool bUpdateAnimation )
{
	m_bUpdateAnimation = bUpdateAnimation;
}

void CBaseAnimated::TransitionAnimation( unsigned int uiAnimationIndex, float flAnimationTransitionTime )
{
	CModel *pModel = GetModel();
	if (!pModel)
		return;

	m_pAnimations.push_back( pModel->GetAnimation( uiAnimationIndex ) );
	m_flAnimationTimes.push_back( 0.0f );
	m_flAnimationTransitionFactors.push_back( 0.0f );
	m_flAnimationTransitionTimes.push_back( flAnimationTransitionTime );
}

void CBaseAnimated::SetAnimation( unsigned int uiAnimationIndex )
{
	CModel *pModel = GetModel();
	if (!pModel)
		return;

	m_pAnimations.clear();
	m_flAnimationTimes.clear();
	m_flAnimationTransitionFactors.clear();
	m_flAnimationTransitionTimes.clear();

	m_pAnimations.push_back( pModel->GetAnimation( uiAnimationIndex ) );
	m_flAnimationTimes.push_back( 0.0f );
}

void CBaseAnimated::SetAnimationTimeScale( float flAnimationTimeScale )
{
	m_flAnimationTimeScale = flAnimationTimeScale;
}