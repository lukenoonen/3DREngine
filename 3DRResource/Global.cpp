#include "Global.h"
#include "FileManager.h"

void CreateGlobals( void )
{
	pFileManager = new CFileManager();
}

void DestroyGlobals( void )
{
	delete pFileManager;
}

CFileManager *pFileManager;