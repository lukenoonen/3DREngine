#ifndef SHAREDASSET_H
#define SHAREDASSET_H

#include "SharedGlobal.h"

enum class ETextureType : EBaseEnum
{
	t_2d = 0,
	t_cubemap,

	i_count,
	i_invalid = i_count,
};

static const char *g_sTextureTypeNames[] =
{
	"2d",
	"cubemap",
};

ETextureType UTIL_TextureTypeNameToEnum( const char *sTextureTypeName );
const char *UTIL_TextureTypeEnumToName( ETextureType eTextureType );

enum class ETextureWrap : unsigned char
{
	t_repeat = 0,
	t_mirrored,
	t_edge,
	t_border,

	i_count,
	i_invalid = i_count,
};

static const char *g_sTextureWrapNames[] =
{
	"repeat",
	"mirrored",
	"edge",
	"border",
};

ETextureWrap UTIL_TextureWrapNameToEnum( const char *sTextureWrapName );
const char *UTIL_TextureWrapEnumToName( ETextureWrap eTextureWrap );

enum class EMaterialType : unsigned char
{
	t_lit = 0,
	t_unlit,
	t_skybox,

	i_count,
	i_invalid = i_count,
};

static const char *g_sMaterialTypeNames[] =
{
	"lit",
	"unlit",
	"skybox",
};

EMaterialType UTIL_MaterialTypeNameToEnum( const char *sMaterialTypeName );
const char *UTIL_MaterialTypeEnumToName( EMaterialType eMaterialType );

#endif // SHAREDASSET_H