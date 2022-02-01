#include "PointLight.h"
#include "RenderManager.h"

CPointLight::CPointLight()
{
	m_flConstant = 1.0f;
	m_flLinear = 0.09f;
	m_flQuadratic = 0.032f;
		
	m_flMaxRadius = 51.3501f;
}

void CPointLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( ERenderPass::t_litpoint );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightposition, 0, &GetPosition() );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 0, &m_flConstant );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 1, &m_flLinear );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 2, &m_flQuadratic );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightmaxdistance, 0, &m_flMaxRadius );
}

void CPointLight::SetShadowCamera( CPointShadowCamera *pPointShadowCamera )
{
	BaseClass::SetShadowCamera( pPointShadowCamera );
}

void CPointLight::SetConstant( float flConstant )
{
	m_flConstant = flConstant;
	CalculateMaxRadius();
}

void CPointLight::SetLinear( float flLinear )
{
	m_flLinear = flLinear;
	CalculateMaxRadius();
}

void CPointLight::SetQuadratic( float flQuadratic )
{
	m_flQuadratic = flQuadratic;
	CalculateMaxRadius();
}

void CPointLight::CalculateMaxRadius( void )
{
	m_flMaxRadius = (-m_flConstant + std::sqrtf( m_flLinear * m_flLinear - 4.0f * m_flQuadratic * (m_flQuadratic - 100.0f * GetMaxDiffuse()) )) / (2.0f * m_flQuadratic);
}