#include "FileManager.h"

EFileType UTIL_FileTypeExtensionToEnum( const char *sFileTypeExtension )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EFileType::i_count; i++)
	{
		if (UTIL_streq( g_sFileTypeExtensions[i], sFileTypeExtension ))
			return (EFileType)i;
	}

	return EFileType::i_invalid;
}

const char *UTIL_FileTypeEnumToExtension( EFileType eFileType )
{
	return g_sFileTypeExtensions[(EBaseEnum)eFileType];
}

EFileType UTIL_FileTypePrePathToEnum( const char *sFileTypePrePath )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EFileType::i_count; i++)
	{
		if (UTIL_streq( g_sFileTypePrePaths[i], sFileTypePrePath ))
			return (EFileType)i;
	}

	return EFileType::i_invalid;
}

const char *UTIL_FileTypeEnumToPrePath( EFileType eFileType )
{
	return g_sFileTypePrePaths[(EBaseEnum)eFileType];
}

CFileManager::CFileManager()
{

}

CFileManager::~CFileManager()
{
	while (!m_pFiles.empty())
	{
		delete m_pFiles.top();
		m_pFiles.pop();
	}
}

bool CFileManager::ReadEntireFile( const char *sFilePath, char *&sData )
{
	if (!OpenFile( sFilePath ))
		return false;

	m_pFiles.top()->pFile->ignore( std::numeric_limits<std::streamsize>::max() );
	unsigned int uiSize = (unsigned int)m_pFiles.top()->pFile->gcount();
	m_pFiles.top()->pFile->clear();
	m_pFiles.top()->pFile->seekg( 0, std::ios_base::beg );

	bool bSuccess = Read( sData, uiSize );
	CloseFile();

	return bSuccess;
}

bool CFileManager::OpenFile( const char *sFilePath )
{
	const char *sExtension = UTIL_extn( sFilePath );
	if (!sExtension)
		return false;

	EFileType eFileType = UTIL_FileTypeExtensionToEnum( sFilePath );

	if (eFileType == EFileType::i_invalid)
		return false;

	int iFlags = std::ios::in;

	switch (eFileType)
	{
	case EFileType::t_animation:
	case EFileType::t_geometry:
	case EFileType::t_image:
	case EFileType::t_material:
	case EFileType::t_texture:
	case EFileType::t_skeleton:
	{
		iFlags |= std::ios::binary;
		break;
	}
	}

	SFile *pTopFile = new SFile; // TODO: clean this up

	char *sFullPath = UTIL_stradd( UTIL_FileTypeEnumToPrePath( eFileType ), sFilePath );
	pTopFile->pFile = new std::fstream( sFullPath, iFlags );
	delete[] sFullPath;

	if (!pTopFile->pFile->is_open())
	{
		delete pTopFile;
		return false;
	}

	pTopFile->eFileType = eFileType;

	m_pFiles.push( pTopFile );

	return true;
}

bool CFileManager::CloseFile( void )
{
	if (m_pFiles.empty())
		return false;

	delete m_pFiles.top();
	m_pFiles.pop();
	return true;
}

bool CFileManager::Read( char *&sData )
{
	unsigned int uiSize;
	return UTIL_Read( *m_pFiles.top()->pFile, &uiSize, 1, unsigned int ) && Read( sData, uiSize );
}

bool CFileManager::Read( unsigned char *&sData )
{
	unsigned int uiSize;
	return UTIL_Read( *m_pFiles.top()->pFile, &uiSize, 1, unsigned int ) && Read( sData, uiSize );
}

bool CFileManager::Read( char *&sData, unsigned int uiSize )
{
	sData = new char[uiSize + 1];
	if (!UTIL_Read( *m_pFiles.top()->pFile, sData, uiSize, char ))
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
	if (!UTIL_Read( *m_pFiles.top()->pFile, sData, uiSize, char ))
	{
		delete[] sData;
		return false;
	}

	sData[uiSize] = '\0';
	return true;
}

EFileType CFileManager::GetFileType( void )
{
	return m_pFiles.top()->eFileType;
}