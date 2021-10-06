#include "PointLight.h"
#include "RenderManager.h"
#include "ShaderManager.h"

CPointLight::CPointLight( float flConstant, float flLinear, float flQuadratic, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flConstant = flConstant;
	m_flLinear = flLinear;
	m_flQuadratic = flQuadratic;

	m_flMaxRadius = (-flLinear + std::sqrtf( flLinear * flLinear - 4.0f * flQuadratic * (flConstant - (1.0f / 0.01f) * std::fmaxf( std::fmaxf( vecDiffuse.r, vecDiffuse.g ), vecDiffuse.b )) )) / (2.0f * flQuadratic);
}

void CPointLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( RENDERPASS_LIT_POINT );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOSITION, 0, &GetPosition() );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOINT, 0, &m_flConstant );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOINT, 1, &m_flLinear );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTPOINT, 2, &m_flQuadratic );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTMAXDISTANCE, 0, &m_flMaxRadius );
}

float CPointLight::GetMaxRadius( void ) const
{
	return m_flMaxRadius;
}