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

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightdirection, 0, &(GetRotation() * g_vec3Front) );
}