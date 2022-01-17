#include "DirLight.h"
#include "RenderManager.h"
#include "ShaderManager.h"

CDirLight::CDirLight( CDirShadowCamera *pDirShadowCamera, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular ) : BaseClass( pDirShadowCamera, vecAmbient, vecDiffuse, vecSpecular )
{

}

void CDirLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( RENDERPASS_LIT_DIR );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTDIRECTION, 0, &(GetRotation() * g_vecFront) );
}