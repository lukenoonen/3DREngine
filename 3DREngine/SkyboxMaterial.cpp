#include "SkyboxMaterial.h"
#include "RenderManager.h"

CSkyboxMaterial::CSkyboxMaterial()
{
	m_pSkybox = NULL;
}

bool CSkyboxMaterial::Init( void )
{
	if (!BaseClass::Init())
		return false;

	return true;
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

	pRenderManager->SetUniform( "u_sSkybox", m_pSkybox->Bind() );
}

void CSkyboxMaterial::SetSkybox( CCubemapTexture *pSkybox )
{
	m_pSkybox = pSkybox;
}