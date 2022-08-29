#include "Global.h"
#include "AssetManager.h"
#include "RenderManager.h"
#include "CommandManager.h"
#include "EntityManager.h"
#include "TimeManager.h"
#include "InputManager.h"

void CreateGlobals( void )
{
	pAssetManager = new CAssetManager();
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
	delete pAssetManager;
}

CAssetManager *pAssetManager;
CRenderManager *pRenderManager;
CCommandManager *pCommandManager;
CEntityManager *pEntityManager;
CTimeManager *pTimeManager;
CInputManager *pInputManager;