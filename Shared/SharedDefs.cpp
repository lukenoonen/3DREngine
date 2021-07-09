#include "SharedDefs.h"
#include "UTIL.h"

ShaderType_t UTIL_ShaderNameToType( const char *sShaderName )
{
	for (int i = 0; i < SHADERTYPE_COUNT; i++)
	{
		if (UTIL_strcmp( g_sShaderNames[i], sShaderName ) == 0)
			return (ShaderType_t)i;
	}

	return SHADERTYPE_INVALID;
}

const char *UTIL_ShaderTypeToName( ShaderType_t tShaderType )
{
	return g_sShaderNames[tShaderType];
}