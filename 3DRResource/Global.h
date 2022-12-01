#ifndef GLOBAL_H
#define GLOBAL_H

#define PROJECT_RESOURCE

#include "SharedGlobal.h"

class CFileManager;

void CreateGlobals( void );
void DestroyGlobals( void );

extern CFileManager *pFileManager;

#endif // GLOBAL_H