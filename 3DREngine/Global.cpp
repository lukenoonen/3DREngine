#include "Global.h"
#include "FileManager.h"
#include "RenderManager.h"
#include "AssetManager.h"
#include "CommandManager.h"
#include "EntityManager.h"
#include "GlobalManager.h"
#include "InputManager.h"

void CreateGlobals( void )
{
	pFileManager = new CFileManager();
	pRenderManager = new CRenderManager();
	pAssetManager = new CAssetManager();
	pCommandManager = new CCommandManager();
	pEntityManager = new CEntityManager();
	pGlobalValues = new CGlobalValues();
	pInputManager = new CInputManager();
}

void DestroyGlobals( void )
{
	delete pInputManager;
	delete pGlobalValues;
	delete pEntityManager;
	delete pCommandManager;
	delete pAssetManager;
	delete pRenderManager;
	delete pFileManager;
}

CFileManager *pFileManager;
CRenderManager *pRenderManager;
CAssetManager *pAssetManager;
CCommandManager *pCommandManager;
CEntityManager *pEntityManager;
CGlobalValues *pGlobalValues;
CInputManager *pInputManager;