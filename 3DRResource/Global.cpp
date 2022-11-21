#include "Global.h"
#include "FileManager.h"
#include "ResourceManager.h"

void CreateGlobals( void )
{
	pFileManager = new CFileManager();
	pResourceManager = new CResourceManager();
}

void DestroyGlobals( void )
{
	delete pResourceManager;
	delete pFileManager;
}

CFileManager *pFileManager;
CResourceManager *pResourceManager;