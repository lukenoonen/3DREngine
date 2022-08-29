#include "File.h"

CFile::CFile( const char *sFileName, EFileType eFileType )
{
	m_pFile = NULL;
	if (eFileType == EFileType::i_invalid)
		return;

	char *sFileNameAndExtension = UTIL_StringAdd( sFileName, g_sFileTypeExtensions[(EBaseEnum)eFileType] );
	char *sFilePath = UTIL_StringAdd( g_sFileTypePrePaths[(EBaseEnum)eFileType], sFileNameAndExtension );

	m_pFile = new std::fstream( sFilePath, g_iFileTypeFlags[(EBaseEnum)eFileType] );

	delete[] sFilePath;
	delete[] sFileNameAndExtension;

	if (!m_pFile->is_open())
	{
		delete m_pFile;
		m_pFile = NULL;
	}
}

CFile::~CFile()
{
	if (m_pFile)
	{
		m_pFile->close();
		delete m_pFile;
	}
}

bool CFile::Success( void ) const
{
	return m_pFile != NULL;
}

bool CFile::Buffer( char *&sBuffer ) const
{
	std::streampos spInitialPosition = m_pFile->tellg();
	m_pFile->ignore( std::numeric_limits<std::streamsize>::max() );
	unsigned int uiSize = (unsigned int)m_pFile->gcount();
	m_pFile->clear();
	m_pFile->seekg( spInitialPosition, std::ios_base::beg );

	sBuffer = new char[uiSize + 1];
	if (!Read( sBuffer, sizeof( char ) * uiSize ))
	{
		delete[] sBuffer;
		return false;
	}

	sBuffer[uiSize] = '\0';
	return true;
}

bool CFile::Write( void *pData, unsigned int uiSize ) const
{
	return UTIL_Read( *m_pFile, pData, uiSize ); // Maybe perform checks to see if the file can be read from/to
}

bool CFile::Read( void *pData, unsigned int uiSize ) const
{
	return UTIL_Read( *m_pFile, pData, uiSize ); // Maybe perform checks to see if the file can be read from/to
}