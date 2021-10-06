#include "UnlitMaterial.h"
#include "ShaderManager.h"
#include "AssetManager.h"

CUnlitMaterial::CUnlitMaterial( CTexture *pDiffuse, const glm::vec2 &vecTextureScale, const char *sPath ) : BaseClass( sPath )
{
	m_pDiffuse = pDiffuse;

	m_pDiffuse->Activate();

	m_vecTextureScale = vecTextureScale;

	SetShaderType( RENDERPASS_DEPTH, SHADERTYPE_DEPTH );
	SetShaderType( RENDERPASS_UNLIT, SHADERTYPE_UNLIT );
}

CUnlitMaterial::~CUnlitMaterial()
{
	m_pDiffuse->Inactivate();

	pAssetManager->CheckTexture( m_pDiffuse );
}

void CUnlitMaterial::Use( void )
{
	BaseClass::Use();

	pShaderManager->SetValue( "u_vecTextureScale", m_vecTextureScale );

	pShaderManager->SetValue( "u_sDiffuse", pAssetManager->BindTexture( m_pDiffuse->GetID(), GL_TEXTURE_2D ) );
}