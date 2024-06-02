#ifndef GEOMETRYRESOURCEDATA_H
#define GEOMETRYRESOURCEDATA_H

#include "Global.h"
#include "Vertex3D.h"

struct SGeometryResourceData
{
	std::vector<SVertex3D> verVertices;
	std::vector<unsigned int> uiIndices;
};

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, SGeometryResourceData &dData );
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, SGeometryResourceData &dData );
#endif // PROJECT_RESOURCE

#endif // GEOMETRYRESOURCEDATA_H