#include "SkyboxMaterial.h"
#include "RenderManager.h"

DEFINE_DATADESC( CSkyboxMaterial )

	DEFINE_FIELD( LinkedDataField, CHandle<CCubemap>, m_hCubemap, "cubemap", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CSkyboxMaterial, material_skybox )

CSkyboxMaterial::CSkyboxMaterial()
{

}

EShaderType CSkyboxMaterial::GetShaderType( void )
{
	switch (pRenderManager->GetRenderPass())
	{
	case ERenderPass::t_unlit:
		return EShaderType::t_skybox;
	}

	return BaseClass::GetShaderType();
}

void CSkyboxMaterial::Apply( void )
{
	BaseClass::Apply();

	pRenderManager->SetUniform( "u_sSkybox", m_hCubemap->Bind() ); // TODO: change uniform name
}