#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Global.h"
#include <fstream>

enum class EFileType : EBaseEnum
{
	t_config = 0,
	t_vertexshader,
	t_geometryshader,
	t_fragmentshader,
	t_animation,
	t_geometry,
	t_image,
	t_material,
	t_texture,
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
	"3mt",
	"3tx",
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
	"resources/materials/",
	"resources/textures/",
	"resources/skeletons/",
};

EFileType UTIL_FileTypeExtensionToEnum( const char *sFileTypeExtension );
const char *UTIL_FileTypeEnumToExtension( EFileType eFileType );

EFileType UTIL_FileTypePrePathToEnum( const char *sFileTypePrePath );
const char *UTIL_FileTypeEnumToPrePath( EFileType eFileType );

struct SFile
{
	~SFile()
	{
		delete pFile;
	}

	std::fstream *pFile;
	EFileType eFileType;
};

class CFileManager
{
public:
	DECLARE_CLASS_NOBASE( CFileManager );

	CFileManager();
	~CFileManager();

	bool ReadEntireFile( const char *sFilePath, char *&sData ); // TODO: Clean this up

	bool OpenFile( const char *sFilePath );
	bool CloseFile( void );

	template <class T>
	bool Read( T &tData );
	bool Read( char *&sData );
	bool Read( unsigned char *&sData );
	bool Read( char *&sData, unsigned int uiSize );
	bool Read( unsigned char *&sData, unsigned int uiSize );

	EFileType GetFileType( void );

private:
	std::stack<SFile *> m_pFiles;
};

template <class T>
bool CFileManager::Read( T &tData )
{
	if (!UTIL_Read( *m_pFiles.top(), &tData, 1, T ))
		return false;

	return true;
}

#endif // FILEMANAGER_H