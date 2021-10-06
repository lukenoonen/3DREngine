#ifndef GLOBAL_H
#define GLOBAL_H

#include "PreGlobal.h"

class CRenderManager;
class CAssetManager;
class CCommandManager;
class CEntityManager;
class CFileManager;
class CGlobalValues;
class CInputManager;
class CShaderManager;

void CreateGlobals( void );
void DestroyGlobals( void );
void ExitError( const char *sError );

extern CRenderManager *pRenderManager;
extern CAssetManager *pAssetManager;
extern CCommandManager *pCommandManager;
extern CEntityManager *pEntityManager;
extern CFileManager *pFileManager;
extern CGlobalValues *pGlobalValues;
extern CInputManager *pInputManager;
extern CShaderManager *pShaderManager;

#endif // GLOBAL_H