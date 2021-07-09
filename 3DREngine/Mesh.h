#ifndef MESH_H
#define MESH_H

#include "Global.h"
#include "Material.h"

struct SVertex
{
	glm::vec3 vecPosition;
	glm::vec3 vecNormal;
	glm::vec3 vecTangent;
	glm::vec3 vecBitangent;
	glm::vec2 vecTexCoords;

	glm::ivec4 vecBoneIDs;
	glm::vec4 vecWeights;
};	

class CMesh
{
public:
	DECLARE_CLASS_NOBASE( CMesh );

	CMesh( const std::vector<SVertex> &verVertices, const std::vector<unsigned int> &uiIndices );
	~CMesh();

	void Draw( void );

private:
	std::vector<SVertex> m_verVertices;
	std::vector<unsigned int> m_uiIndices;

	unsigned int m_uiVAO;
	unsigned int m_uiVBO;
	unsigned int m_uiEBO;
};

#endif // MESH_H