#include "ColorHUDMaterial.h"

CColorHUDMaterial::CColorHUDMaterial( glm::vec3 &vecColor, const char *sPath, unsigned int uiDrawFlags ) : BaseClass( sPath, uiDrawFlags )
{
	m_vecColor = vecColor;
}

bool CColorHUDMaterial::Use( void )
{
	if (!BaseClass::Use())
		return false;

	CShader *pShader = pShaderManager->GetShader( GetShaderType() );

	pShader->SetValue( "u_vecColor", m_vecColor );

	return true;
}

ShaderType_t CColorHUDMaterial::GetShaderType( void ) const
{
	return SHADERTYPE_COLORHUD;
}