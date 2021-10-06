#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Global.h"
#include "BaseAsset.h"

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

class CGeometry : public CBaseAsset
{
public:
	DECLARE_CLASS( CGeometry, CBaseAsset );

	CGeometry( const std::vector<SVertex> &verVertices, const std::vector<unsigned int> &uiIndices, const char *sPath );
	virtual ~CGeometry();

	void Draw( void );

private:
	std::vector<SVertex> m_verVertices;
	std::vector<unsigned int> m_uiIndices;

	unsigned int m_uiVAO;
	unsigned int m_uiVBO;
	unsigned int m_uiEBO;
};

#endif // GEOMETRY_H