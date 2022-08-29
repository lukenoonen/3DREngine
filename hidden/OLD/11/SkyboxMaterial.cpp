#include "SkyboxMaterial.h"
#include "RenderManager.h"
#include "AssetManager.h"

CSkyboxMaterial::CSkyboxMaterial( CTexture *pSkybox, const char *sPath ) : BaseClass( sPath )
{
	m_pSkybox = pSkybox;

	m_pSkybox->Activate();

	SetShaderType( ERenderPass::t_unlit, EShaderType::t_skybox );
}

CSkyboxMaterial::~CSkyboxMaterial()
{
	m_pSkybox->Inactivate();

	pAssetManager->CheckTexture( m_pSkybox );
}

void CSkyboxMaterial::Use( void )
{
	BaseClass::Use();

	pRenderManager->SetUniform( "u_sSkybox", pAssetManager->BindTexture( m_pSkybox->GetID(), GL_TEXTURE_CUBE_MAP ) );
}