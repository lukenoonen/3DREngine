#include "SkyboxMaterial.h"
#include "ShaderManager.h"
#include "AssetManager.h"

CSkyboxMaterial::CSkyboxMaterial( CTexture *pSkybox, const char *sPath ) : BaseClass( sPath )
{
	m_pSkybox = pSkybox;

	m_pSkybox->Activate();

	SetShaderType( RENDERPASS_UNLIT, SHADERTYPE_SKYBOX );
}

CSkyboxMaterial::~CSkyboxMaterial()
{
	m_pSkybox->Inactivate();

	pAssetManager->CheckTexture( m_pSkybox );
}

void CSkyboxMaterial::Use( void )
{
	BaseClass::Use();

	pShaderManager->SetValue( "u_sSkybox", pAssetManager->BindTexture( m_pSkybox->GetID(), GL_TEXTURE_CUBE_MAP ) );
}