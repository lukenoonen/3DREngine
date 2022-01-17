#include "CSMLight.h"
#include "ShaderManager.h"
#include "EntityManager.h"
#include "RenderManager.h"
#include "CommandManager.h"
#include "AssetManager.h"

CCSMLight::CCSMLight( CCSMShadowCamera *pCSMShadowCamera, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular ) : BaseClass( pCSMShadowCamera, vecAmbient, vecDiffuse, vecSpecular )
{

}

void CCSMLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( RENDERPASS_LIT_CSM );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTDIRECTION, 0, &(GetRotation() * g_vecFront) );
}