#ifndef FILETYPE_H
#define FILETYPE_H

#include "Global.h"

// TODO: work out a simplier system for maps for now (just text stuff basically)
enum class EFileType : EBaseEnum
{
	t_animation,
	t_cubemap,
	t_font,
	t_geometry,
	t_skeleton,
	t_texture,

	i_count,
	i_invalid = i_count,
};

static const char *g_sFileTypeExtensions[] =
{
	".ani",
	".cub",
	".fnt",
	".geo",
	".ske",
	".tex",
};

static const char *g_sFileTypePrePaths[] =
{
	"resources/animations/",
	"resources/cubemaps/",
	"resources/fonts/",
	"resources/geometry/",
	"resources/skeletons/",
	"resources/textures/",
};

static int g_iFileTypeFlags[] =
{
	std::ios::out | std::ios::binary,
	std::ios::out | std::ios::binary,
	std::ios::out | std::ios::binary,
	std::ios::out | std::ios::binary,
	std::ios::out | std::ios::binary,
	std::ios::out | std::ios::binary,
};

#endif // FILETYPE_H