#include "LitMaterial.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "AssetManager.h"

CLitMaterial::CLitMaterial( CTexture *pDiffuse, CTexture *pSpecular, CTexture *pNormal, float flShininess, const glm::vec2 &vecTextureScale, const char *sPath ) : BaseClass( sPath )
{
	m_pDiffuse = pDiffuse;
	m_pSpecular = pSpecular;
	m_pNormal = pNormal;

	m_pDiffuse->Activate();
	m_pSpecular->Activate();
	m_pNormal->Activate();

	m_flShininess = flShininess;

	m_vecTextureScale = vecTextureScale;

	SetShaderType( RENDERPASS_SHADOW_DIR, SHADERTYPE_SHADOW_DIR );
	SetShaderType( RENDERPASS_SHADOW_POINT, SHADERTYPE_SHADOW_POINT );
	SetShaderType( RENDERPASS_SHADOW_SPOT, SHADERTYPE_SHADOW_SPOT );
	SetShaderType( RENDERPASS_SHADOW_CSM, SHADERTYPE_SHADOW_CSM );
	SetShaderType( RENDERPASS_DEPTH, SHADERTYPE_DEPTH );
	SetShaderType( RENDERPASS_LIT_DIR, SHADERTYPE_LIT_DIR );
	SetShaderType( RENDERPASS_LIT_POINT, SHADERTYPE_LIT_POINT );
	SetShaderType( RENDERPASS_LIT_SPOT, SHADERTYPE_LIT_SPOT );
	SetShaderType( RENDERPASS_LIT_CSM, SHADERTYPE_LIT_CSM );
}

CLitMaterial::~CLitMaterial()
{
	m_pDiffuse->Inactivate();
	m_pSpecular->Inactivate();
	m_pNormal->Inactivate();

	pAssetManager->CheckTexture( m_pDiffuse );
	pAssetManager->CheckTexture( m_pSpecular );
	pAssetManager->CheckTexture( m_pNormal );
}

void CLitMaterial::Use( void )
{
	BaseClass::Use();

	RenderPass_t tRenderPass = pRenderManager->GetRenderPass();
	switch (tRenderPass)
	{
	case RENDERPASS_LIT_DIR:
	case RENDERPASS_LIT_POINT:
	case RENDERPASS_LIT_SPOT:
	case RENDERPASS_LIT_CSM:
	{
		pShaderManager->SetValue( "u_vecTextureScale", m_vecTextureScale );

		pShaderManager->SetValue( "u_sDiffuse", pAssetManager->BindTexture( m_pDiffuse->GetID(), GL_TEXTURE_2D ) );

		if (pShaderManager->GetShaderQuality() != SHADERQUALITY_LOW)
		{
			if (m_pSpecular)
			{
				pShaderManager->SetValue( "u_bUseSpecular", true );
				pShaderManager->SetValue( "u_sSpecular", pAssetManager->BindTexture( m_pSpecular->GetID(), GL_TEXTURE_2D ) );
			}
			else
			{
				pShaderManager->SetValue( "u_bUseSpecular", false );
			}
		}
		if (m_pNormal)
		{
			pShaderManager->SetValue( "u_bUseNormal", true );
			pShaderManager->SetValue( "u_sNormal", pAssetManager->BindTexture( m_pNormal->GetID(), GL_TEXTURE_2D ) );
		}
		else
		{
			pShaderManager->SetValue( "u_bUseNormal", false );
		}
		pShaderManager->SetValue( "u_flShininess", m_flShininess );

		if (pShaderManager->GetShaderShadow() == SHADERSHADOW_TRUE)
		{
			pShaderManager->SetValue( "u_sShadowMap", pRenderManager->GetShadowMapIndex() );
		}
	}
	}
}