#ifndef GLOBAL_H
#define GLOBAL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "SharedGlobal.h"

class CFileManager;
class CRenderManager;
class CAssetManager;
class CCommandManager;
class CEntityManager;
class CGlobalValues;
class CInputManager;

void CreateGlobals( void );
void DestroyGlobals( void );

extern CFileManager *pFileManager;
extern CRenderManager *pRenderManager;
extern CAssetManager *pAssetManager;
extern CCommandManager *pCommandManager;
extern CEntityManager *pEntityManager;
extern CGlobalValues *pGlobalValues;
extern CInputManager *pInputManager;

#endif // GLOBAL_H