#include "BaseAnimated.h"
#include "RenderManager.h"
#include "GlobalManager.h"
#include "AssetManager.h"

CBaseAnimated::CBaseAnimated()
{
	m_bAnimate = false;
	m_bUpdateAnimation = false;

	m_flAnimationTimeScale = 1.0f;
}

void CBaseAnimated::Exit( void )
{
	m_pSkeleton->Inactivate();
	pAssetManager->CheckSkeleton( m_pSkeleton );

	for (unsigned int i = 0; i < (unsigned int)m_pAnimationList.size(); i++)
	{
		m_pAnimationList[i]->Inactivate();
		pAssetManager->CheckAnimation( m_pAnimationList[i] );
	}

	BaseClass::Exit();
}

void CBaseAnimated::PostThink( void )
{
	if (m_bAnimate && m_bUpdateAnimation)
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

		m_pSkeleton->UpdateAnimation( m_matBoneTransforms, m_pAnimations, m_flAnimationTimes, m_flAnimationTransitionFactors );
	}

	BaseClass::PostThink();
}

void CBaseAnimated::PreDraw( void )
{
	if (m_bAnimate)
	{
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_animate, (EBaseEnum)EShaderPreprocessorAnimate::t_true );
		pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_bones, 0, 0, (unsigned int)UTIL_Min( (unsigned int)m_matBoneTransforms.size(), 64 ), &m_matBoneTransforms[0] );
	}

	BaseClass::PreDraw();
}

void CBaseAnimated::PostDraw( void )
{
	if (m_bAnimate)
		pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_animate, (EBaseEnum)EShaderPreprocessorAnimate::t_false );

	BaseClass::PostDraw();
}

void CBaseAnimated::SetSkeleton( const char *sPath )
{
	CSkeleton *pSkeleton = pAssetManager->GetSkeleton( sPath );
	pSkeleton->Activate();
	pSkeleton->SetUpBoneTransforms( m_matBoneTransforms );
	m_pSkeleton = pSkeleton;
}

void CBaseAnimated::AddAnimation( const char *sPath )
{
	CAnimation *pAnimation = pAssetManager->GetAnimation( sPath );
	pAnimation->Activate();
	m_pAnimationList.push_back( pAnimation );
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
	m_pAnimations.push_back( m_pAnimationList[uiAnimationIndex] );
	m_flAnimationTimes.push_back( 0.0f );
	m_flAnimationTransitionFactors.push_back( 0.0f );
	m_flAnimationTransitionTimes.push_back( flAnimationTransitionTime );
}

void CBaseAnimated::SetAnimation( unsigned int uiAnimationIndex )
{
	m_pAnimations.clear();
	m_flAnimationTimes.clear();
	m_flAnimationTransitionFactors.clear();
	m_flAnimationTransitionTimes.clear();

	m_pAnimations.push_back( m_pAnimationList[uiAnimationIndex] );
	m_flAnimationTimes.push_back( 0.0f );
}

void CBaseAnimated::SetAnimationTimeScale( float flAnimationTimeScale )
{
	m_flAnimationTimeScale = flAnimationTimeScale;
}