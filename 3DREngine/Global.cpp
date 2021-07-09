#include "Global.h"
#include "AnimationManager.h"
#include "EntityManager.h"
#include "FileManager.h"
#include "GlobalManager.h"
#include "InputManager.h"
#include "MaterialManager.h"
#include "ModelManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

void CreateGlobals( void )
{
	pRenderManager = new CRenderManager();
	pAnimationManager = new CAnimationManager();
	pEntityManager = new CEntityManager();
	pGlobalValues = new CGlobalValues();
	pInputManager = new CInputManager();
	pMaterialManager = new CMaterialManager();
	pModelManager = new CModelManager();
	pShaderManager = new CShaderManager();
	pTextureManager = new CTextureManager();
	pFileManager = new CFileManager();
}

void DestroyGlobals( void )
{
	delete pFileManager;
	delete pTextureManager;
	delete pShaderManager;
	delete pModelManager;
	delete pMaterialManager;
	delete pInputManager;
	delete pGlobalValues;
	delete pEntityManager;
	delete pAnimationManager;
	delete pRenderManager;
}

CAnimationManager *pAnimationManager;
CEntityManager *pEntityManager;
CFileManager *pFileManager;
CGlobalValues *pGlobalValues;
CInputManager *pInputManager;
CMaterialManager *pMaterialManager;
CModelManager *pModelManager;
CRenderManager *pRenderManager;
CShaderManager *pShaderManager;
CTextureManager *pTextureManager;