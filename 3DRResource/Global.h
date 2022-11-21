#ifndef GLOBAL_H
#define GLOBAL_H

#define PROJECT_RESOURCE

#include "SharedGlobal.h"

class CResourceManager;
class CFileManager;

void CreateGlobals( void );
void DestroyGlobals( void );

extern CResourceManager *pResourceManager;
extern CFileManager *pFileManager;

#endif // GLOBAL_H