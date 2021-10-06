#include "SharedAsset.h"
#include "UTIL.h"

TextureType_t UTIL_TextureTypeNameToEnum( const char *sTextureTypeName )
{
	for (unsigned int i = 0; i < (unsigned int)TEXTURETYPE_COUNT; i++)
	{
		if (UTIL_streq( g_sTextureTypeNames[i], sTextureTypeName ))
			return (TextureType_t)i;
	}

	return TEXTURETYPE_INVALID;
}

const char *UTIL_TextureTypeEnumToName( TextureType_t tTextureType )
{
	return g_sTextureTypeNames[tTextureType];
}

TextureWrap_t UTIL_TextureWrapNameToEnum( const char *sTextureWrapName )
{
	for (unsigned int i = 0; i < (unsigned int)TEXTUREWRAP_COUNT; i++)
	{
		if (UTIL_streq( g_sTextureWrapNames[i], sTextureWrapName ))
			return (TextureWrap_t)i;
	}

	return TEXTUREWRAP_INVALID;
}

const char *UTIL_TextureWrapEnumToName( TextureWrap_t tTextureWrap )
{
	return g_sTextureWrapNames[tTextureWrap];
}

MaterialType_t UTIL_MaterialTypeNameToEnum( const char *sMaterialTypeName )
{
	for (unsigned int i = 0; i < (unsigned int)MATERIALTYPE_INVALID; i++)
	{
		if (UTIL_streq( g_sMaterialTypeNames[i], sMaterialTypeName ))
			return (MaterialType_t)i;
	}

	return MATERIALTYPE_INVALID;
}

const char *UTIL_MaterialTypeEnumToName( MaterialType_t tMaterialType )
{
	return g_sMaterialTypeNames[tMaterialType];
}