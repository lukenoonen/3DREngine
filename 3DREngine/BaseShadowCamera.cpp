#include "BaseShadowCamera.h"
#include "AssetManager.h"

CBaseShadowCamera::CBaseShadowCamera( float flFadeNear, float flFadeFar, float flBlurScale, unsigned int uiBaseSize, float flSizeRatio, unsigned int uiRenderPriority ) : BaseClass( uiBaseSize, flSizeRatio, uiRenderPriority )
{
	m_flFadeNear = flFadeNear;
	m_flFadeFar = flFadeFar;

	m_flBlurScale = flBlurScale;
}

void CBaseShadowCamera::ActivateLight( void )
{
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 0, &m_flFadeNear );
	pShaderManager->SetUniformBufferObject( UBO_SHADOWFADE, 1, &m_flFadeFar );

	pShaderManager->SetUniformBufferObject( UBO_SHADOWBLUR, 0, &m_flBlurScale );
}

void CBaseShadowCamera::SetBlurScale( float flBlurScale )
{
	m_flBlurScale = flBlurScale;
}