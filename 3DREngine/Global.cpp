#include "Global.h"
#include "TimeManager.h"
#include "FileManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "CommandManager.h"
#include "EntityManager.h"
#include "InputManager.h"

void CreateGlobals( void )
{
	pTimeManager = new CTimeManager();
	pFileManager = new CFileManager();
	pResourceManager = new CResourceManager();
	pRenderManager = new CRenderManager();
	pCommandManager = new CCommandManager();
	pEntityManager = new CEntityManager();
	pInputManager = new CInputManager();
}

void DestroyGlobals( void )
{
	delete pInputManager;
	delete pEntityManager;
	delete pCommandManager;
	delete pRenderManager;
	delete pResourceManager;
	delete pFileManager;
	delete pTimeManager;
}

CFileManager *pFileManager;
CResourceManager *pResourceManager;
CRenderManager *pRenderManager;
CCommandManager *pCommandManager;
CEntityManager *pEntityManager;
CTimeManager *pTimeManager;
CInputManager *pInputManager;