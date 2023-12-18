#ifndef TEXTURERESOURCEDATA_H
#define TEXTURERESOURCEDATA_H

#include "Global.h"

enum class ETextureWrap : EBaseEnum
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

DEFINE_ENUM_NAMES( ETextureWrap, g_sTextureWrapNames )

DEFINE_ENUM_GETVALUE( ETextureWrap )

struct STextureResourceData
{
	bool bFiltered;
	ETextureWrap eTextureWrap;
	glm::vec4 vec4BorderColor;
	int iWidth;
	int iHeight;
	unsigned char ucChannels;
	unsigned char *pData;
};

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, STextureResourceData &dData );
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, STextureResourceData &dData );
#endif // PROJECT_RESOURCE

#endif // TEXTURERESOURCEDATA_H