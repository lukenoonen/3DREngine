#include "BaseShadowCamera.h"
#include "AssetManager.h"

CBaseShadowCamera::CBaseShadowCamera()
{
	m_flFadeNear = 225.0f;
	m_flFadeFar = 250.0f;

	m_flBlurScale = 0.0f;
}

void CBaseShadowCamera::ActivateLight( void )
{
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 0, &m_flFadeNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 1, &m_flFadeFar );

	pShaderManager->SetUniformBufferObject( UBO_SHADOWBLUR, 0, &m_flBlurScale );
}

void CBaseShadowCamera::SetFadeNear( float flFadeNear )
{
	m_flFadeNear = flFadeNear;
}

void CBaseShadowCamera::SetFadeFar( float flFadeFar )
{
	m_flFadeFar = flFadeFar;
}

void CBaseShadowCamera::SetBlurScale( float flBlurScale )
{
	m_flBlurScale = flBlurScale;
}