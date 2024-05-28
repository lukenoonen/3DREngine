#ifndef GLOBAL_H
#define GLOBAL_H

#define PROJECT_ENGINE

#include "SharedGlobal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

// TODO: clean this up

class CTimeManager;
class CResourceManager;
class CRenderManager;
class CCommandManager;
class CEntityManager;
class CInputManager;
class CFileManager;

void CreateGlobals( void );
void DestroyGlobals( void );

extern CResourceManager *pResourceManager;
extern CRenderManager *pRenderManager;
extern CCommandManager *pCommandManager;
extern CEntityManager *pEntityManager;
extern CInputManager *pInputManager;
extern CFileManager *pFileManager;
extern CTimeManager *pTimeManager;

#endif // GLOBAL_H