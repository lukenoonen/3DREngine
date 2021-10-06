#include "Material.h"
#include "GlobalManager.h"
#include "ShaderManager.h"

CMaterial::CMaterial( const char *sPath ) : BaseClass( sPath )
{
	for (unsigned int i = 0; i < (unsigned int)RENDERPASS_COUNT; i++)
		m_tShaderTypes[i] = SHADERTYPE_INVALID;
}

bool CMaterial::ShouldDraw( void ) const
{
	return GetShaderType() != SHADERTYPE_INVALID;
}

void CMaterial::Use( void )
{
	pShaderManager->Use( GetShaderType() );
}

ShaderType_t CMaterial::GetShaderType( void ) const
{
	return m_tShaderTypes[pRenderManager->GetRenderPass()];
}

void CMaterial::SetShaderType( RenderPass_t tRenderPass, ShaderType_t tShaderType )
{
	m_tShaderTypes[tRenderPass] = tShaderType;
}