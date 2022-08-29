#ifndef SHAREDFILE_H
#define SHAREDFILE_H

#include "SharedGlobal.h"

enum class EFileType : EBaseEnum
{
	t_config = 0,
	t_vertexshader,
	t_geometryshader,
	t_fragmentshader,
	t_animation,
	t_geometry,
	t_image,
	t_skeleton,

	i_count,
	i_invalid = i_count,
};

static const char *g_sFileTypeExtensions[] =
{
	"cfg",
	"vs",
	"gs",
	"fs",
	"3an",
	"3gm",
	"3im",
	"3sk",
};

static const char *g_sFileTypePrePaths[] =
{
	"resources/config/",
	"resources/shaders/",
	"resources/shaders/",
	"resources/shaders/",
	"resources/animations/",
	"resources/geometry/",
	"resources/images/",
	"resources/skeletons/",
};

EFileType UTIL_FileTypeExtensionToEnum( const char *sFileTypeExtension );
const char *UTIL_FileTypeEnumToExtension( EFileType eFileType );

EFileType UTIL_FileTypePrePathToEnum( const char *sFileTypePrePath );
const char *UTIL_FileTypeEnumToPrePath( EFileType eFileType );

#endif // SHAREDFILE_H