#ifndef FILETYPE_H
#define FILETYPE_H

#include "Global.h"

// TODO: work out a simplier system for maps for now (just text stuff basically)
enum class EFileType : EBaseEnum
{
	t_config = 0,

	t_vertexshader,
	t_geometryshader,
	t_fragmentshader,
	t_headershader,

	t_text,

	t_animation,
	t_cubemap,
	t_geometry,
	t_skeleton,
	t_texture,

	t_entity,

	t_map,

	i_count,
	i_invalid = i_count,
};

static const char *g_sFileTypeExtensions[] =
{
	".cfg",

	".vs",
	".gs",
	".fs",
	".sh",

	".txt",

	".ani",
	".cub",
	".geo",
	".ske",
	".tex",

	".ent",

	".map",
};

static const char *g_sFileTypePrePaths[] =
{
	"resources/config/",

	"resources/shaders/",
	"resources/shaders/",
	"resources/shaders/",
	"resources/shaders/",

	"resources/text/",

	"resources/animations/",
	"resources/cubemaps/",
	"resources/geometry/",
	"resources/skeletons/",
	"resources/textures/",

	"resources/entities/",

	"resources/maps/",
};

static int g_iFileTypeFlags[] =
{
	std::ios::in,

	std::ios::in,
	std::ios::in,
	std::ios::in,
	std::ios::in,

	std::ios::in,

	std::ios::in | std::ios::binary,
	std::ios::in | std::ios::binary,
	std::ios::in | std::ios::binary,
	std::ios::in | std::ios::binary,
	std::ios::in | std::ios::binary,

	std::ios::in | std::ios::binary,

	std::ios::in | std::ios::binary,
};

#endif // FILETYPE_H