#include "FileManager.h"

CFileManager::CFileManager()
{

}

CFileManager::~CFileManager()
{
	while (Close()) continue;
}

bool CFileManager::Open( const char *sFileName, EFileType eFileType )
{
	char *sFileNameAndExtension = UTIL_stradd( sFileName, g_sFileTypeExtensions[(EBaseEnum)eFileType] );
	char *sFilePath = UTIL_stradd( g_sFileTypePrePaths[(EBaseEnum)eFileType], sFileNameAndExtension );

	m_fFileReaders.emplace();
	bool bSuccess = m_fFileReaders.top().ReadFile( sFilePath, g_iFileTypeFlags[(EBaseEnum)eFileType] );

	delete[] sFilePath;
	delete[] sFileNameAndExtension;

	if (!bSuccess)
		m_fFileReaders.pop();

	return bSuccess;
}

bool CFileManager::Close( void )
{
	if (m_fFileReaders.empty())
		return false;

	m_fFileReaders.pop();
	return true;
}

bool CFileManager::Buffer( char *&sBuffer )
{
	if (m_fFileReaders.top().GetFile()->Buffer( sBuffer ))
		return true;

	Close();
	return false;
}

bool CFileManager::WriteBytes( void *pData, unsigned int uiSize )
{
	if (m_fFileReaders.top().GetFile()->WriteBytes( pData, uiSize ))
		return true;

	Close();
	return false;
}

bool CFileManager::ReadBytes( void *pData, unsigned int uiSize )
{
	if (m_fFileReaders.top().GetFile()->ReadBytes( pData, uiSize ))
		return true;

	Close();
	return false;
}