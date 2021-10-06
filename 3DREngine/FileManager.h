#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Global.h"
#include <fstream>

class CFileManager
{
public:
	DECLARE_CLASS_NOBASE( CFileManager );

	CFileManager();
	~CFileManager();

	bool ReadEntireFile( const char *sFilePath, char *&sData );

	bool OpenFile( const char *sFilePath );
	bool CloseFile( void );

	template <class T>
	bool Read( T &tData );
	bool Read( char *&sData );
	bool Read( unsigned char *&sData );
	bool Read( char *&sData, unsigned int uiSize );
	bool Read( unsigned char *&sData, unsigned int uiSize );

private:
	std::stack<std::fstream *> m_pFiles;
};

template <class T>
bool CFileManager::Read( T &tData )
{
	if (!UTIL_Read( *m_pFiles.top(), &tData, 1, T ))
		return false;

	return true;
}

#endif // FILEMANAGER_H