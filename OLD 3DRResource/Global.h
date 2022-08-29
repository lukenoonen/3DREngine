#ifndef GLOBAL_H
#define GLOBAL_H

#define PROJECT_RESOURCE

#include "BaseGlobal.h"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

class CFileManager;
class CAssetManager;

void CreateGlobals( void );
void DestroyGlobals( void );

extern CFileManager *pFileManager;
extern CAssetManager *pAssetManager;

#endif // GLOBAL_H