#include "DirLight.h"
#include "RenderManager.h"

CDirLight::CDirLight()
{

}

void CDirLight::SetShadowCamera( CDirShadowCamera *pDirShadowCamera )
{
	BaseClass::SetShadowCamera( pDirShadowCamera );
}

void CDirLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( ERenderPass::t_litdir );

	glm::vec3 vec3Direction = GetRotation() * g_vec3Front;
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightdirection, 0, &vec3Direction );
}