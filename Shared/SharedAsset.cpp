#include "SharedAsset.h"
#include "UTIL.h"

ETextureType UTIL_TextureTypeNameToEnum( const char *sTextureTypeName )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)ETextureType::i_count; i++)
	{
		if (UTIL_streq( g_sTextureTypeNames[i], sTextureTypeName ))
			return (ETextureType)i;
	}

	return ETextureType::i_invalid;
}

const char *UTIL_TextureTypeEnumToName( ETextureType eTextureType )
{
	return g_sTextureTypeNames[(EBaseEnum)eTextureType];
}

ETextureWrap UTIL_TextureWrapNameToEnum( const char *sTextureWrapName )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)ETextureWrap::i_count; i++)
	{
		if (UTIL_streq( g_sTextureWrapNames[i], sTextureWrapName ))
			return (ETextureWrap)i;
	}

	return ETextureWrap::i_invalid;
}

const char *UTIL_TextureWrapEnumToName( ETextureWrap eTextureWrap )
{
	return g_sTextureWrapNames[(EBaseEnum)eTextureWrap];
}

EMaterialType UTIL_MaterialTypeNameToEnum( const char *sMaterialTypeName )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EMaterialType::i_count; i++)
	{
		if (UTIL_streq( g_sMaterialTypeNames[i], sMaterialTypeName ))
			return (EMaterialType)i;
	}

	return EMaterialType::i_invalid;
}

const char *UTIL_MaterialTypeEnumToName( EMaterialType eMaterialType )
{
	return g_sMaterialTypeNames[(EBaseEnum)eMaterialType];
}