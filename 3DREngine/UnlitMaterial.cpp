#include "UnlitMaterial.h"
#include "TextureManager.h"
#include "ShaderManager.h"

CUnlitMaterial::CUnlitMaterial( CTexture *pDiffuse, const glm::vec2 &vecTextureScale, const char *sPath, unsigned int uiDrawFlags ) : BaseClass( sPath, uiDrawFlags )
{
	m_pDiffuse = pDiffuse;
	m_vecTextureScale = vecTextureScale;
}

bool CUnlitMaterial::Use( void )
{
	if (!BaseClass::Use())
		return false;

	CShader *pShader = pShaderManager->GetShader( GetShaderType() );

	pShader->SetValue( "u_vecTextureScale", m_vecTextureScale );

	pShader->SetValue( "u_sDiffuse", pTextureManager->BindTexture( m_pDiffuse->GetID(), GL_TEXTURE_2D ) );
	return true;
}

ShaderType_t CUnlitMaterial::GetShaderType( void ) const
{
	return SHADERTYPE_UNLIT;
}