#include "DirLight.h"
#include "RenderManager.h"
#include "DirShadowCamera.h"

DEFINE_LINKED_CLASS( CDirLight, light_dir )

CDirLight::CDirLight()
{

}

bool CDirLight::Init( void )
{
	if (!BaseClass::Init())
		return false;

	CBaseShadowCamera *pShadowCamera = GetShadowCamera();
	if (pShadowCamera && !(dynamic_cast<CDirShadowCamera *>(pShadowCamera)))
		return false;

	return true;
}

void CDirLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( ERenderPass::t_litdir );

	glm::vec3 vec3Direction = GetRotation() * g_vec3Front;
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightdirection, 0, &vec3Direction );
}