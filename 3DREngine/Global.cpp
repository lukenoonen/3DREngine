#include "Global.h"
#include "FileManager.h"
#include "ResourceManager.h"
#include "RenderManager.h"
#include "CommandManager.h"
#include "EntityManager.h"
#include "TimeManager.h"
#include "InputManager.h"

void CreateGlobals( void )
{
	pFileManager = new CFileManager();
	pResourceManager = new CResourceManager();
	pRenderManager = new CRenderManager();
	pCommandManager = new CCommandManager();
	pEntityManager = new CEntityManager();
	pTimeManager = new CTimeManager();
	pInputManager = new CInputManager();
}

void DestroyGlobals( void )
{
	delete pInputManager;
	delete pTimeManager;
	delete pEntityManager;
	delete pCommandManager;
	delete pRenderManager;
	delete pResourceManager;
	delete pFileManager;
}

CFileManager *pFileManager;
CResourceManager *pResourceManager;
CRenderManager *pRenderManager;
CCommandManager *pCommandManager;
CEntityManager *pEntityManager;
CTimeManager *pTimeManager;
CInputManager *pInputManager;