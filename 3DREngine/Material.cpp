#include "Material.h"
#include "GlobalManager.h"
#include "ShaderManager.h"
#include <UTIL.h>

CMaterial::CMaterial( const char *sPath, unsigned int uiDrawFlags )
{
	m_sPath = UTIL_stredit( sPath );
	m_uiDrawFlags = uiDrawFlags;
}

CMaterial::~CMaterial()
{
	delete[] m_sPath;
}

bool CMaterial::Use( void )
{
	CShader *pOverrideShader = pShaderManager->GetOverrideShader();
	if (pOverrideShader)
	{
		pOverrideShader->Use();
		return false;
	}

	CShader *pShader = pShaderManager->GetShader( GetShaderType() );
	if (!pShader)
		return false;

	pShader->Use();
	return true;
}

const char *CMaterial::GetPath( void ) const
{
	return m_sPath;
}

bool CMaterial::ShouldDraw( unsigned int uiDrawFlags ) const
{
	return (m_uiDrawFlags & uiDrawFlags) != 0;
}

unsigned int CMaterial::GetDrawFlags( void ) const
{
	return m_uiDrawFlags;
}

ShaderType_t CMaterial::GetShaderType( void ) const
{
	return SHADERTYPE_INVALID;
}