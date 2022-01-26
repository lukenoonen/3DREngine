#include "SpotLight.h"
#include "RenderManager.h"
#include "ShaderManager.h"

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

	pRenderManager->SetRenderPass( RENDERPASS_LIT_SPOT );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOSITION, 0, &GetPosition() );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOINT, 0, &m_flConstant );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOINT, 1, &m_flLinear );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOINT, 2, &m_flQuadratic );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTMAXDISTANCE, 0, &m_flMaxRadius );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTDIRECTION, 0, &(GetRotation() * g_vecFront) );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTSPOT, 0, &m_flCutoff );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTSPOT, 1, &m_flOuterCutoff );
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