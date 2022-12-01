#ifndef GEOMETRYRESOURCEDATA_H
#define GEOMETRYRESOURCEDATA_H

#include "Global.h"

struct SVertex
{
	glm::vec3 vec3Position;
	glm::vec3 vec3Normal;
	glm::vec3 vec3Tangent;
	glm::vec3 vec3Bitangent;
	glm::vec2 vec2TexCoords;
	glm::ivec4 vec4BoneIDs;
	glm::vec4 vec4Weights;
};

struct SGeometryResourceData
{
	std::vector<SVertex> verVertices;
	std::vector<unsigned int> uiIndices;
};

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, SGeometryResourceData &dData );
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, SGeometryResourceData &dData );
bool UTIL_GetValue( const CTextItem *pTextItem, SGeometryResourceData &dValue );
#endif // PROJECT_RESOURCE

#endif // GEOMETRYRESOURCEDATA_H