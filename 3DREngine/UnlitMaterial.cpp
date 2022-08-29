#include "UnlitMaterial.h"

CUnlitMaterial::CUnlitMaterial()
{
	m_pDiffuse = NULL;

	m_vec2TextureScale = g_vec2One;
}

bool CUnlitMaterial::Init( void )
{
	if (!BaseClass::Init())
		return false;

	return true;
}

EShaderType CUnlitMaterial::GetShaderType( void )
{
	switch (pRenderManager->GetRenderPass())
	{
	case ERenderPass::t_depth:
		return EShaderType::t_depth;
	case ERenderPass::t_unlit:
		return EShaderType::t_unlit;
	}

	return BaseClass::GetShaderType();
}

void CUnlitMaterial::Apply( void )
{
	BaseClass::Apply();

	pRenderManager->SetUniform( "u_vecTextureScale", m_vec2TextureScale );

	pRenderManager->SetUniform( "u_sDiffuse", m_pDiffuse->Bind() );
}

void CUnlitMaterial::SetDiffuse( CFlatTexture *pDiffuse )
{
	m_pDiffuse = pDiffuse;
}

void CUnlitMaterial::SetTextureScale( const glm::vec2 &vec2TextureScale )
{
	m_vec2TextureScale = vec2TextureScale;
}