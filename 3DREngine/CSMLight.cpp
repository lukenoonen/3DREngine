#include "CSMLight.h"
#include "ShaderManager.h"
#include "EntityManager.h"
#include "RenderManager.h"
#include "CommandManager.h"
#include "AssetManager.h"

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

	pRenderManager->SetRenderPass( RENDERPASS_LIT_CSM );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTDIRECTION, 0, &(GetRotation() * g_vecFront) );
}