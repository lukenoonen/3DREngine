#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "Global.h"
#include "BaseAsset.h"

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

class CGeometry : public CBaseAsset
{
public:
	DECLARE_CLASS( CGeometry, CBaseAsset );

	CGeometry( const std::vector<SVertex> &verVertices, const std::vector<unsigned int> &uiIndices, const char *sPath );
	virtual ~CGeometry();

	virtual EAssetType GetAssetType( void ) const;

	void Draw( void );

private:
	std::vector<SVertex> m_verVertices;
	std::vector<unsigned int> m_uiIndices;

	unsigned int m_uiVAO;
	unsigned int m_uiVBO;
	unsigned int m_uiEBO;
};

#endif // GEOMETRY_H