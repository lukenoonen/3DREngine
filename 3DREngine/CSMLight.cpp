#include "CSMLight.h"
#include "RenderManager.h"

CCSMLight::CCSMLight()
{

}

void CCSMLight::SetShadowCamera( CCSMShadowCamera *pCSMShadowCamera )
{
	BaseClass::SetShadowCamera( pCSMShadowCamera );
}

void CCSMLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( ERenderPass::t_litcsm );

	glm::vec3 vec3Direction = GetRotation() * g_vec3Front;
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightdirection, 0, &vec3Direction );
}