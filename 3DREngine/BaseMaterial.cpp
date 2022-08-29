#include "BaseMaterial.h"

CBaseMaterial::CBaseMaterial()
{

}

EShaderType CBaseMaterial::GetShaderType( void )
{
	return EShaderType::i_invalid;
}

bool CBaseMaterial::ShouldApply( void ) // TODO: go through all these files and add const where possible
{
	return GetShaderType() != EShaderType::i_invalid;
}

void CBaseMaterial::Apply( void )
{
	pRenderManager->UseShader( GetShaderType() );
}