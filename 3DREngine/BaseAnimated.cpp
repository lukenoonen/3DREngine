#include "BaseAnimated.h"
#include "RenderManager.h"
#include "GlobalManager.h"

CBaseAnimated::CBaseAnimated( const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_bAnimate = true;
	m_bUpdateAnimation = false;
	m_bValidAnimationIndex = false;

	m_flAnimationTime = 0.0f;
	m_flAnimationTimeScale = 1.0f;
}

void CBaseAnimated::PostThink( void )
{
	if (m_bValidAnimationIndex && m_bAnimate && m_bUpdateAnimation)
	{
		CModel *pModel = GetModel();
		if (pModel->IsAnimated())
			m_flAnimationTime = pModel->UpdateAnimation( m_matBoneTransforms, m_uiAnimationIndex, m_flAnimationTime, m_flAnimationTimeScale );
	}

	BaseClass::PostThink();
}

void CBaseAnimated::PreDraw( void )
{
	BaseClass::PreDraw();

	pShaderManager->SetShaderAnimate( SHADERANIMATE_TRUE );

	if (m_bValidAnimationIndex && m_bAnimate)
	{
		CModel *pModel = GetModel();
		if (pModel->IsAnimated())
			pShaderManager->SetUniformBufferObject( UBO_BONES, 0, 0, (unsigned int)UTIL_Min( (unsigned int)m_matBoneTransforms.size(), 64 ), &m_matBoneTransforms[0] );
	}
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

void CBaseAnimated::SetAnimationIndex( unsigned int uiAnimationIndex )
{
	CModel *pModel = GetModel();

	if (m_bValidAnimationIndex)
	{
		if (pModel->IsValidAnimation( uiAnimationIndex ))
			m_uiAnimationIndex = uiAnimationIndex;
		else
			m_bValidAnimationIndex = false;
	}
	else
	{
		if (pModel->IsValidAnimation( uiAnimationIndex ))
		{
			m_uiAnimationIndex = uiAnimationIndex;
			m_bValidAnimationIndex = true;
		}
	}
}

void CBaseAnimated::SetAnimationTime( float flAnimationTime )
{
	m_flAnimationTime = flAnimationTime;
}

void CBaseAnimated::SetAnimationTimeScale( float flAnimationTimeScale )
{
	m_flAnimationTimeScale = flAnimationTimeScale;
}