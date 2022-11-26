#ifndef GEOMETRYRESOURCE_H
#define GEOMETRYRESOURCE_H

#include "Global.h"
#include "BaseResource.h"

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

class CGeometryResource : public CBaseResource
{
public:
	DECLARE_CLASS( CGeometryResource, CBaseResource )

	CGeometryResource( const std::vector<SVertex> &verVertices, const std::vector<unsigned int> &uiIndices, const char *sName );
	virtual ~CGeometryResource();

	void Draw( void ) const; // TODO: clarify language between "draw", "render", etc

private:
	std::vector<SVertex> m_verVertices;
	std::vector<unsigned int> m_uiIndices;

	GLuint m_glVAO;
	GLuint m_glVBO;
	GLuint m_glEBO;
};

bool UTIL_Write( CFile *pFile, CGeometryResource *&pData );
bool UTIL_Read( CFile *pFile, CGeometryResource *&pData );
bool UTIL_GetValue( const CTextItem *pTextItem, CGeometryResource *&pValue );

#endif // GEOMETRYRESOURCE_H