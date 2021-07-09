#include "SkyboxMaterial.h"
#include "TextureManager.h"
#include "ShaderManager.h"

CSkyboxMaterial::CSkyboxMaterial( CTexture *pSkybox, const char *sPath, unsigned int uiDrawFlags ) : BaseClass( sPath, uiDrawFlags )
{
	m_pSkybox = pSkybox;
}

bool CSkyboxMaterial::Use( void )
{
	if (!BaseClass::Use())
		return false;

	CShader *pShader = pShaderManager->GetShader( GetShaderType() );

	pShader->SetValue( "u_sSkybox", pTextureManager->BindTexture( m_pSkybox->GetID(), GL_TEXTURE_CUBE_MAP ) );
	return true;
}

ShaderType_t CSkyboxMaterial::GetShaderType( void ) const
{
	return SHADERTYPE_SKYBOX;
}