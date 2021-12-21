#include "SpotLight.h"
#include "RenderManager.h"
#include "ShaderManager.h"

CSpotLight::CSpotLight( float flCutoff, float flOuterCutoff, float flConstant, float flLinear, float flQuadratic, CSpotShadowCamera *pSpotShadowCamera, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( pSpotShadowCamera, vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flConstant = flConstant;
	m_flLinear = flLinear;
	m_flQuadratic = flQuadratic;

	m_flMaxRadius = (-flLinear + std::sqrtf( flLinear * flLinear - 4.0f * flQuadratic * (flConstant - 100.0f * std::fmaxf( std::fmaxf( vecDiffuse.r, vecDiffuse.g ), vecDiffuse.b )) )) / (2.0f * flQuadratic);

	m_flCutoff = glm::cos( flCutoff );
	m_flOuterCutoff = glm::cos( flOuterCutoff );
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