#include "FileManager.h"
#include <fstream>
#include <Windows.h>

bool CC_Load( const std::vector<const char *> &command )
{
	if (command.size() == 0)
	{
		char sFilename[MAX_PATH];
		ZeroMemory( &sFilename, sizeof( sFilename ) );

		OPENFILENAME ofn;
		ZeroMemory( &ofn, sizeof( ofn ) );
		ofn.lStructSize = sizeof( ofn );
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = "\0Config Files\0*.cfg";
		ofn.lpstrFile = sFilename;
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = "Select a File";
		ofn.lpstrDefExt = ".sit";
		ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (GetOpenFileNameA( &ofn ))
			pFileManager->Load( sFilename );
	}
	else
	{
		pFileManager->Load( command[0] );
	}
	return true;
}
CConCommand cc_load( "load", CC_Load, "load (file name)" );

CFileManager::CFileManager()
{
	Load( "resources/config/autoexec.cfg" );
}

void CFileManager::Load( const char *sFileName )
{
	const char *pExtension = UTIL_extn( sFileName );

	bool bSuccess = false;

	if (pExtension)
	{
		if (UTIL_strcmp( "cfg", pExtension ) == 0)
			bSuccess = LoadConfig( sFileName );
	}

	//if (!bSuccess)
	//	pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_red, "Unable to load file %s!", sFileName );
}

bool CFileManager::LoadConfig( const char *sFileName )
{
	if (UTIL_strcmp( "cfg", UTIL_extn( sFileName ) ) != 0)
		return false;

	// pHUDManager->AddConsoleEntry( FONTTYPE_NORMAL, c_white, "Loading config file %s...", sFileName );

	std::fstream fConfigFile( sFileName, std::ios::in );

	if (!fConfigFile.is_open())
		return false;

	int iLength = 0;
	while (fConfigFile.get() != EOF)
		iLength++;

	fConfigFile.clear();
	fConfigFile.seekg( 0, fConfigFile.beg );

	char *sFileContents = new char[iLength + 1];
	fConfigFile.read( sFileContents, iLength );
	sFileContents[iLength] = '\0';

	fConfigFile.close();

	CCommandProcesser::ProcessCommand( sFileContents );

	delete[] sFileContents;
	return true;
}