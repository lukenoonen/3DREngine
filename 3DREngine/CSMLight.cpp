#include "CSMLight.h"
#include "RenderManager.h"
#include "EntityManager.h"
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

	pRenderManager->SetRenderPass( ERenderPass::t_litcsm );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightdirection, 0, &(GetRotation() * g_vec3Front) );
}