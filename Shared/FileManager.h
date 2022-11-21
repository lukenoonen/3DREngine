#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Global.h"

// TODO: work out a simplier system for maps for now (just text stuff basically)
enum class EFileType : EBaseEnum
{
	t_config = 0,

	t_vertexshader,
	t_geometryshader,
	t_fragmentshader,
	t_headershader,

	t_animation,
	t_cubemap,
	t_geometry,
	t_skeleton,
	t_texture,

	t_entitytext,
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

	".ani",
	".cub",
	".geo",
	".ske",
	".tex",

	".txt", // TODO: clean this up
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

	"resources/animations/",
	"resources/cubemaps/",
	"resources/geometry/",
	"resources/skeletons/",
	"resources/textures/",

	"resources/entities/", // TODO: clean this up
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
	std::ios::in,
	std::ios::in,
	std::ios::in,
	std::ios::in,

	std::ios::in,
	 
	std::ios::in | std::ios::binary,

	std::ios::in | std::ios::binary,
};

class CFileManager
{
public:
	DECLARE_CLASS_NOBASE( CFileManager )

	CFileManager();
	~CFileManager();

	bool Open( const char *sFileName, EFileType eFileType );
	bool Close( void );

	bool Buffer( char *&sBuffer );

	template <class T> bool Write( T &tData );
	template <class T> bool Read( T &tData );

	bool WriteBytes( void *pData, unsigned int uiSize );
	bool ReadBytes( void *pData, unsigned int uiSize );

private:
	std::stack<CFileReader> m_fFileReaders;
};

template <class T> bool CFileManager::Write( T &tData )
{
	if (m_fFileReaders.top().GetFile()->Read( tData ))
		return true;

	Close();
	return false;
}

template <class T> bool CFileManager::Read( T &tData )
{
	if (m_fFileReaders.top().GetFile()->Write( tData ))
		return true;

	Close();
	return false;
}

#endif // FILEMANAGER_H