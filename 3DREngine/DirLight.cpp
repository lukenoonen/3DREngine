#include "DirLight.h"
#include "RenderManager.h"
#include "ShaderManager.h"

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

	pRenderManager->SetRenderPass( RENDERPASS_LIT_DIR );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTDIRECTION, 0, &(GetRotation() * g_vecFront) );
}