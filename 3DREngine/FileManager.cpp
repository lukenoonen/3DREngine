#include "FileManager.h"

CFileManager::CFileManager()
{

}

CFileManager::~CFileManager()
{
	while (!m_pFiles.empty())
	{
		m_pFiles.top()->close();
		delete m_pFiles.top();
		m_pFiles.pop();
	}
}

bool CFileManager::ReadEntireFile( const char *sFilePath, char *&sData )
{
	if (!OpenFile( sFilePath ))
		return false;

	m_pFiles.top()->ignore( std::numeric_limits<std::streamsize>::max() );
	unsigned int uiSize = (unsigned int)m_pFiles.top()->gcount();
	m_pFiles.top()->clear();
	m_pFiles.top()->seekg( 0, std::ios_base::beg );

	bool bSuccess = Read( sData, uiSize );
	CloseFile();

	return bSuccess;
}

#include <iostream>

bool CFileManager::OpenFile( const char *sFilePath )
{
	const char *sExtension = UTIL_extn( sFilePath );
	if (!sExtension)
		return false;

	int iFlags = std::ios::in;

	const char *sPrePath = NULL;
	if (UTIL_streq( sExtension, "cfg" ))
		sPrePath = "resources/config/";
	else if (UTIL_streq( sExtension, "vs" ) || UTIL_streq( sExtension, "gs" ) || UTIL_streq( sExtension, "fs" ) || UTIL_streq( sExtension, "sh" ))
		sPrePath = "resources/shaders/";
	else
	{
		iFlags |= std::ios::binary;
		if (UTIL_streq( sExtension, "3an" ))
			sPrePath = "resources/animations/";
		else if (UTIL_streq( sExtension, "3gm" ))
			sPrePath = "resources/geometry/";
		else if (UTIL_streq( sExtension, "3im" ))
			sPrePath = "resources/images/";
		else if (UTIL_streq( sExtension, "3mt" ))
			sPrePath = "resources/materials/";
		else if (UTIL_streq( sExtension, "3ms" ))
			sPrePath = "resources/meshes/";
		else if (UTIL_streq( sExtension, "3md" ))
			sPrePath = "resources/models/";
		else if (UTIL_streq( sExtension, "3tx" ))
			sPrePath = "resources/textures/";
		else if (UTIL_streq( sExtension, "3sk" ))
			sPrePath = "resources/skeletons/";
	}

	if (!sPrePath)
		return false;

	char *sFullPath = UTIL_stradd( sPrePath, sFilePath );
	m_pFiles.push( new std::fstream( sFullPath, iFlags ) );
	delete[] sFullPath;

	if (m_pFiles.top()->is_open())
		return true;

	delete m_pFiles.top();
	m_pFiles.pop();
	return false;
}

bool CFileManager::CloseFile( void )
{
	if (m_pFiles.empty())
		return false;

	m_pFiles.top()->close();
	delete m_pFiles.top();
	m_pFiles.pop();
	return true;
}

bool CFileManager::Read( char *&sData )
{
	unsigned int uiSize;
	return UTIL_Read( *m_pFiles.top(), &uiSize, 1, unsigned int ) && Read( sData, uiSize );
}

bool CFileManager::Read( unsigned char *&sData )
{
	unsigned int uiSize;
	return UTIL_Read( *m_pFiles.top(), &uiSize, 1, unsigned int ) && Read( sData, uiSize );
}

bool CFileManager::Read( char *&sData, unsigned int uiSize )
{
	sData = new char[uiSize + 1];
	if (!UTIL_Read( *m_pFiles.top(), sData, uiSize, char ))
	{
		delete[] sData;
		return false;
	}

	sData[uiSize] = '\0';
	return true;
}

bool CFileManager::Read( unsigned char *&sData, unsigned int uiSize )
{
	sData = new unsigned char[uiSize + 1];
	if (!UTIL_Read( *m_pFiles.top(), sData, uiSize, char ))
	{
		delete[] sData;
		return false;
	}

	sData[uiSize] = '\0';
	return true;
}