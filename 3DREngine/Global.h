#ifndef GLOBAL_H
#define GLOBAL_H

#include "PreGlobal.h"
#include "CommandProcessor.h"

class CAnimationManager;
class CEntityManager;
class CFileManager;
class CGlobalValues;
class CInputManager;
class CMaterialManager;
class CModelManager;
class CRenderManager;
class CShaderManager;
class CTextureManager;

void CreateGlobals( void );
void DestroyGlobals( void );

extern CAnimationManager *pAnimationManager;
extern CEntityManager *pEntityManager;
extern CFileManager *pFileManager;
extern CGlobalValues *pGlobalValues;
extern CInputManager *pInputManager;
extern CMaterialManager *pMaterialManager;
extern CModelManager *pModelManager;
extern CRenderManager *pRenderManager;
extern CShaderManager *pShaderManager;
extern CTextureManager *pTextureManager;

#endif // GLOBAL_H