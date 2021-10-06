#ifndef SHAREDASSET_H
#define SHAREDASSET_H

enum TextureType_t : unsigned char
{
	TEXTURETYPE_2D = 0,
	TEXTURETYPE_CUBEMAP,

	TEXTURETYPE_COUNT,
	TEXTURETYPE_INVALID = TEXTURETYPE_COUNT,
};

static const char *g_sTextureTypeNames[] =
{
	"2d",
	"cubemap",
};

TextureType_t UTIL_TextureTypeNameToEnum( const char *sTextureTypeName );
const char *UTIL_TextureTypeEnumToName( TextureType_t tTextureType );

enum TextureWrap_t : unsigned char
{
	TEXTUREWRAP_REPEAT = 0,
	TEXTUREWRAP_MIRRORED,
	TEXTUREWRAP_EDGE,
	TEXTUREWRAP_BORDER,

	TEXTUREWRAP_COUNT,
	TEXTUREWRAP_INVALID = TEXTUREWRAP_COUNT,
};

static const char *g_sTextureWrapNames[] =
{
	"repeat",
	"mirrored",
	"edge",
	"border",
};

TextureWrap_t UTIL_TextureWrapNameToEnum( const char *sTextureWrapName );
const char *UTIL_TextureWrapEnumToName( TextureWrap_t tTextureWrap );

enum MaterialType_t : unsigned char
{
	MATERIALTYPE_LIT = 0,
	MATERIALTYPE_UNLIT,
	MATERIALTYPE_SKYBOX,
	
	MATERIALTYPE_COUNT,
	MATERIALTYPE_INVALID = MATERIALTYPE_COUNT,
};

static const char *g_sMaterialTypeNames[] =
{
	"lit",
	"unlit",
	"skybox",
};

MaterialType_t UTIL_MaterialTypeNameToEnum( const char *sMaterialTypeName );
const char *UTIL_MaterialTypeEnumToName( MaterialType_t tMaterialType );

#endif // SHAREDASSET_H