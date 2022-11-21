#include "CSMLight.h"
#include "RenderManager.h"
#include "CSMShadowCamera.h"

DEFINE_LINKED_CLASS( CCSMLight, light_csm )

CCSMLight::CCSMLight()
{

}

bool CCSMLight::Init( void )
{
	if (!BaseClass::Init())
		return false;

	CBaseShadowCamera *pShadowCamera = GetShadowCamera();
	if (pShadowCamera && !(dynamic_cast<CCSMShadowCamera *>(pShadowCamera)))
		return false;

	return true;
}

void CCSMLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( ERenderPass::t_litcsm );

	glm::vec3 vec3Direction = GetRotation() * g_vec3Front;
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightdirection, 0, &vec3Direction );
}