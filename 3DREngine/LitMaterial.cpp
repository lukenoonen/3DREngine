#include "LitMaterial.h"
#include "TextureManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"

const char *g_sCSMShadowMapNames[] =
{
	"u_sShadowMap[0]",
	"u_sShadowMap[1]",
	"u_sShadowMap[2]",
	"u_sShadowMap[3]",
};

CLitMaterial::CLitMaterial( CTexture *pDiffuse, CTexture *pSpecular, CTexture *pNormal, float flShininess, const glm::vec2 &vecTextureScale, const char *sPath, unsigned int uiDrawFlags ) : BaseClass( sPath, uiDrawFlags )
{
	m_pDiffuse = pDiffuse;
	m_pSpecular = pSpecular;
	m_pNormal = pNormal;
	m_flShininess = flShininess;

	m_vecTextureScale = vecTextureScale;
}

bool CLitMaterial::Use( void )
{
	if (!BaseClass::Use())
		return false;

	CShader *pShader = pShaderManager->GetShader( GetShaderType() );

	if (UTIL_strcmp( GetPath(), "br1ck.3mt" ) == 0)
	{
		pShader->SetValue( "u_vecTextureScale", g_vec2One );
		pShader->SetValue( "u_sDiffuse", pRenderManager->GetShadowMapIndex( 0 ) );
		pShader->SetValue( "u_bUseSpecular", false );
		pShader->SetValue( "u_bUseNormal", false );
		pShader->SetValue( "u_flShininess", 1.0f );
		if (pShaderManager->GetShaderSubType() == SHADERSUBTYPE_CSM)
		{
			for (unsigned int i = 0; i < 4; i++)
				pShader->SetValue( g_sCSMShadowMapNames[i], pRenderManager->GetShadowMapIndex( i ) );
		}
		else
		{
			pShader->SetValue( "u_sShadowMap", pRenderManager->GetShadowMapIndex( 0 ) );
		}
	}
	else
	{
		pShader->SetValue( "u_vecTextureScale", m_vecTextureScale );

		pShader->SetValue( "u_sDiffuse", pTextureManager->BindTexture( m_pDiffuse->GetID(), GL_TEXTURE_2D ) );

		if (m_pSpecular)
		{
			pShader->SetValue( "u_bUseSpecular", true );
			pShader->SetValue( "u_sSpecular", pTextureManager->BindTexture( m_pSpecular->GetID(), GL_TEXTURE_2D ) );
		}
		else
		{
			pShader->SetValue( "u_bUseSpecular", false );
		}
		if (m_pNormal)
		{
			pShader->SetValue( "u_bUseNormal", true );
			pShader->SetValue( "u_sNormal", pTextureManager->BindTexture( m_pNormal->GetID(), GL_TEXTURE_2D ) );
		}
		else
		{
			pShader->SetValue( "u_bUseNormal", false );
		}
		pShader->SetValue( "u_flShininess", m_flShininess );

		if (pShaderManager->GetShaderSubType() == SHADERSUBTYPE_CSM)
		{
			for (unsigned int i = 0; i < 4; i++)
				pShader->SetValue( g_sCSMShadowMapNames[i], pRenderManager->GetShadowMapIndex( i ) );
		}
		else
		{
			pShader->SetValue( "u_sShadowMap", pRenderManager->GetShadowMapIndex( 0 ) );
		}
	}

	return true;
}

ShaderType_t CLitMaterial::GetShaderType( void ) const
{
	return SHADERTYPE_LIT;
}