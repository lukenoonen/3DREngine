#ifndef GLOBAL_H
#define GLOBAL_H

#define PROJECT_ENGINE

#include "BaseGlobal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// TODO: clean this up

class CAssetManager;
class CRenderManager;
class CCommandManager;
class CEntityManager;
class CTimeManager;
class CInputManager;

void CreateGlobals( void );
void DestroyGlobals( void );

extern CAssetManager *pAssetManager;
extern CRenderManager *pRenderManager;
extern CCommandManager *pCommandManager;
extern CEntityManager *pEntityManager;
extern CTimeManager *pTimeManager;
extern CInputManager *pInputManager;

#endif // GLOBAL_H