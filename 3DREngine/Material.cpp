#include "Material.h"
#include "RenderManager.h"
#include "GlobalManager.h"

CMaterial::CMaterial( const char *sPath ) : BaseClass( sPath )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)ERenderPass::i_count; i++)
		m_tShaderTypes[i] = EShaderType::i_invalid;
}

EAssetType CMaterial::GetAssetType( void ) const
{
	return EAssetType::t_material;
}

void CMaterial::Use( void )
{
	pRenderManager->UseShader( GetShaderType() );
}

bool CMaterial::ShouldDraw( void ) const
{
	return GetShaderType() != EShaderType::i_invalid;
}

EShaderType CMaterial::GetShaderType( void ) const
{
	return m_tShaderTypes[(EBaseEnum)pRenderManager->GetRenderPass()];
}

void CMaterial::SetShaderType( ERenderPass eRenderPass, EShaderType eShaderType )
{
	m_tShaderTypes[(EBaseEnum)eRenderPass] = eShaderType;
}