#include "BaseShadowCamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( CBaseShadowCamera )

	DEFINE_FIELD( DataField, float, m_flFadeNear, "fadenear", 0 )
	DEFINE_FIELD( DataField, float, m_flFadeFar, "fadefar", 0 )
	DEFINE_FIELD( DataField, float, m_flBlurScale, "blurscale", 0 )

END_DATADESC()

CBaseShadowCamera::CBaseShadowCamera()
{
	m_flFadeNear = 225.0f;
	m_flFadeFar = 250.0f;
	m_flBlurScale = 0.0f;
}

void CBaseShadowCamera::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadowfade, 0, &m_flFadeNear );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadowfade, 1, &m_flFadeFar );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_shadowblur, 0, &m_flBlurScale );
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