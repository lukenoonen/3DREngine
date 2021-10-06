#include "SpotLight.h"
#include "RenderManager.h"
#include "ShaderManager.h"

CSpotLight::CSpotLight( float flCutoff, float flOuterCutoff, float flConstant, float flLinear, float flQuadratic, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( flConstant, flLinear, flQuadratic, vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_flCutoff = glm::cos( flCutoff );
	m_flOuterCutoff = glm::cos( flOuterCutoff );
}

void CSpotLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( RENDERPASS_LIT_SPOT );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTDIRECTION, 0, &(GetRotation() * g_vecFront) );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTSPOT, 0, &m_flCutoff );
	pShaderManager->SetUniformBufferObject( UBO_LIGHTSPOT, 1, &m_flOuterCutoff );
}