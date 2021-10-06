#include "Global.h"
#include "RenderManager.h"
#include "AssetManager.h"
#include "CommandManager.h"
#include "EntityManager.h"
#include "FileManager.h"
#include "GlobalManager.h"
#include "InputManager.h"
#include "ShaderManager.h"

void CreateGlobals( void )
{
	pRenderManager = new CRenderManager();
	pAssetManager = new CAssetManager();
	pCommandManager = new CCommandManager();
	pEntityManager = new CEntityManager();
	pFileManager = new CFileManager();
	pGlobalValues = new CGlobalValues();
	pInputManager = new CInputManager();
	pShaderManager = new CShaderManager();
}

void DestroyGlobals( void )
{
	delete pShaderManager;
	delete pInputManager;
	delete pGlobalValues;
	delete pFileManager;
	delete pEntityManager;
	delete pCommandManager;
	delete pAssetManager;
	delete pRenderManager;
}

void ExitError( const char *sError )
{
	char *sFullError = UTIL_stradd( "The program has stopped prematurely due to an error:\n", sError );
	MessageBox( NULL, sFullError, "Error", MB_ICONERROR | MB_OK );
	delete[] sFullError;

	exit( EXIT_FAILURE );
}

CRenderManager *pRenderManager;
CAssetManager *pAssetManager;
CCommandManager *pCommandManager;
CEntityManager *pEntityManager;
CFileManager *pFileManager;
CGlobalValues *pGlobalValues;
CInputManager *pInputManager;
CShaderManager *pShaderManager;