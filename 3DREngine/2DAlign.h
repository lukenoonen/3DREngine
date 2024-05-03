#ifndef _2DALIGN_H
#define _2DALIGN_H

#include "Global.h"

enum class E2DAlign : EBaseEnum
{
	t_topleft = 0,
	t_topcenter,
	t_topright,
	t_middleleft,
	t_middlecenter,
	t_middleright,
	t_bottomleft,
	t_bottomcenter,
	t_bottomright,

	i_count,
	i_invalid = i_count,
};

static const char *g_s2DAlignNames[] =
{
	"topleft",
	"topcenter",
	"topright",
	"middleleft",
	"middlecenter",
	"middleright",
	"bottomleft",
	"bottomcenter",
	"bottomright",
};

DEFINE_ENUM_NAMES( E2DAlign, g_s2DAlignNames )

DEFINE_ENUM_GETVALUE( E2DAlign )

glm::vec2 UTIL_Get2DAlignOffset_ScreenSpace( E2DAlign e2DAlign );
glm::vec2 UTIL_Get2DAlignOffset_ClipSpace( E2DAlign e2DAlign );

#endif // _2DALIGN_H