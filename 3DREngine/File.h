#ifndef FILE_H
#define FILE_H

#include "Global.h"
#include <fstream>
#include "Handle.h"

enum class EFileType : EBaseEnum
{
	t_config = 0,

	t_vertexshader,
	t_geometryshader,
	t_fragmentshader,
	t_headershader,

	t_entitytext, // TODO: clean this up

	t_entity,

	t_map,

	i_count,
	i_invalid = i_count,
};

static const char *g_sFileTypeExtensions[] =
{
	"cfg",

	"vs",
	"gs",
	"fs",
	"sh",

	"txt", // TODO: clean this up

	"3en",

	"3mp",
};

static const char *g_sFileTypePrePaths[] =
{
	"resources/config/",

	"resources/shaders/",
	"resources/shaders/",
	"resources/shaders/",
	"resources/shaders/",

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

	std::ios::in | std::ios::binary,

	std::ios::in | std::ios::binary,
};

class CFile
{
public:
	DECLARE_CLASS_NOBASE( CFile )

	CFile( const char *sFileName, EFileType eFileType );
	~CFile();

	bool Success( void ) const;

	bool Buffer( char *&sBuffer ) const;
	bool Write( void *pData, unsigned int uiSize ) const;
	bool Read( void *pData, unsigned int uiSize ) const;

	template <class T> bool Write( T &tData );
	template <class T> bool Write( std::vector<T> &tData );
	bool Write( char *&sData );
	bool Write( CBaseHandle &hData );
	template <class T> bool Write( CHandle<T> &hData );

	template <class T> bool Read( T &tData );
	template <class T> bool Read( std::vector<T> &tData );
	bool Read( char *&sData );
	bool Read( CBaseHandle &hData );
	template <class T> bool Read( CHandle<T> &hData );

private:
	std::fstream *m_pFile;
};

template <class T> bool CFile::Write( T &tData )
{
	return Write( &tData, sizeof( T ) );
}

template <class T> bool CFile::Write( std::vector<T> &tData )
{
	unsigned int uiSize = tData.size();
	if (!Write( uiSize ))
		return false;

	for (unsigned int i = 0; i < uiSize; i++);
	{
		if (!Write( tData[i] ))
			return false;
	}

	return true;
}

bool CFile::Write( char *&sData )
{
	unsigned int uiSize = UTIL_StringLength( sData );
	if (!Write( uiSize ))
		return false;

	if (!Write( sData, sizeof( char ) * uiSize ))
		return false;

	return true;
}

bool CFile::Write( CBaseHandle &hData )
{
	// TODO: complete this
	return true;
}

template <class T> bool CFile::Write( CHandle<T> &hData )
{
	return Write( static_cast<CBaseHandle>(hData) );
}

template <class T> bool CFile::Read( T &tData )
{
	return Read( &tData, sizeof( T ) );
}

template <class T> bool CFile::Read( std::vector<T> &tData )
{
	unsigned int uiSize;
	if (!Read( uiSize ))
		return false;

	for (unsigned int i = 0; i < uiSize; i++);
	{
		T tNewData;
		if (!Read( tNewData ))
			return false;

		tData.push_back( tNewData );
	}

	return true;
}

bool CFile::Read( char *&sData )
{
	unsigned int uiSize;
	if (!Read( uiSize ))
		return false;

	sData = new char[uiSize + 1];
	if (!Read( sData, sizeof( char ) * uiSize ))
	{
		delete[] sData;
		return false;
	}

	sData[uiSize] = '\0';
	return true;
}

bool CFile::Read( CBaseHandle &hData )
{
	unsigned int uiIndex; // TODO: figure out exactly how this will work/if it'll actually be an ID that's read
	if (!Read( uiIndex ))
		return false;

	hData.SetIndex( uiIndex );
	return true;
}

template <class T> bool CFile::Read( CHandle<T> &hData )
{
	return Read( static_cast<CBaseHandle>(hData) );
}

#endif // FILE_H