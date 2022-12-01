#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include "Global.h"
#include "FileType.h"

class CFileManager
{
public:
	DECLARE_CLASS_NOBASE( CFileManager )

	CFileManager();
	~CFileManager();

	bool Open( const char *sFileName, EFileType eFileType );
	bool Open( const char *sFilePath, int iFlags );
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
	if (m_fFileReaders.top().GetFile()->Write( tData ))
		return true;

	Close();
	return false;
}

template <class T> bool CFileManager::Read( T &tData )
{
	if (m_fFileReaders.top().GetFile()->Read( tData ))
		return true;

	Close();
	return false;
}

#endif // FILEMANAGER_H