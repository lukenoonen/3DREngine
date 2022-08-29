#include "SpotLight.h"
#include "RenderManager.h"

CSpotLight::CSpotLight()
{
	m_pSpotShadowCamera = NULL;

	m_flCutoff = 0.7660f;
	m_flOuterCutoff = 0.7071f;

	m_flConstant = 1.0f;
	m_flLinear = 0.09f;
	m_flQuadratic = 0.032f;

	m_flMaxRadius = 51.3501f;
}

void CSpotLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( ERenderPass::t_litspot );

	glm::vec3 vec3Direction = GetRotation() * g_vec3Front;
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightposition, 0, &GetPosition() );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 0, &m_flConstant );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 1, &m_flLinear );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 2, &m_flQuadratic );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightmaxdistance, 0, &m_flMaxRadius );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightdirection, 0, &vec3Direction );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightspot, 0, &m_flCutoff );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightspot, 1, &m_flOuterCutoff );
}

void CSpotLight::SetShadowCamera( CSpotShadowCamera *pSpotShadowCamera )
{
	m_pSpotShadowCamera = pSpotShadowCamera;

	BaseClass::SetShadowCamera( pSpotShadowCamera );
}

void CSpotLight::SetCutoff( float flCutoff )
{
	m_flCutoff = glm::cos( flCutoff );
}

void CSpotLight::SetOuterCutoff( float flOuterCutoff )
{
	m_flOuterCutoff = glm::cos( flOuterCutoff );

	if (m_pSpotShadowCamera)
		m_pSpotShadowCamera->SetOuterCutoff( flOuterCutoff );
}

void CSpotLight::SetConstant( float flConstant )
{
	m_flConstant = flConstant;
	CalculateMaxRadius();
}

void CSpotLight::SetLinear( float flLinear )
{
	m_flLinear = flLinear;
	CalculateMaxRadius();
}

void CSpotLight::SetQuadratic( float flQuadratic )
{
	m_flQuadratic = flQuadratic;
	CalculateMaxRadius();
}

void CSpotLight::CalculateMaxRadius( void )
{
	m_flMaxRadius = (-m_flConstant + std::sqrtf( m_flLinear * m_flLinear - 4.0f * m_flQuadratic * (m_flQuadratic - 100.0f * GetMaxDiffuse()) )) / (2.0f * m_flQuadratic);
}