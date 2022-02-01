#include "UnlitMaterial.h"
#include "RenderManager.h"
#include "AssetManager.h"

CUnlitMaterial::CUnlitMaterial( CTexture *pDiffuse, const glm::vec2 &vec2TextureScale, const char *sPath ) : BaseClass( sPath )
{
	m_pDiffuse = pDiffuse;
	m_vec2TextureScale = vec2TextureScale;

	m_pDiffuse->Activate();

	SetShaderType( ERenderPass::t_depth, EShaderType::t_depth );
	SetShaderType( ERenderPass::t_unlit, EShaderType::t_unlit );
}

CUnlitMaterial::~CUnlitMaterial()
{
	m_pDiffuse->Inactivate();

	pAssetManager->CheckTexture( m_pDiffuse );
}

void CUnlitMaterial::Use( void )
{
	BaseClass::Use();

	pRenderManager->SetUniform( "u_vecTextureScale", m_vec2TextureScale );

	pRenderManager->SetUniform( "u_sDiffuse", pAssetManager->BindTexture( m_pDiffuse->GetID(), GL_TEXTURE_2D ) );
}