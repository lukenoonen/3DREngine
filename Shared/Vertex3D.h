#ifndef VERTEX3D_H
#define VERTEX3D_H

#include "Global.h"

struct SVertex3D
{
	glm::vec3 vec3Position;
	glm::vec3 vec3Normal;
	glm::vec3 vec3Tangent;
	glm::vec3 vec3Bitangent;
	glm::vec2 vec2TexCoords;
	glm::ivec4 vec4BoneIDs;
	glm::vec4 vec4Weights;
};

#endif // VERTEX3D_H