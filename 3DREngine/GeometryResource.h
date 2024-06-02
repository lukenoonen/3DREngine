#ifndef GEOMETRYRESOURCE_H
#define GEOMETRYRESOURCE_H

#include "Global.h"
#include "BaseResource.h"
#include "GeometryResourceData.h"

class CGeometryResource : public CBaseResource
{
public:
	DECLARE_CLASS( CGeometryResource, CBaseResource )

	CGeometryResource( SGeometryResourceData &sData, const char *sName );
	virtual ~CGeometryResource();

	void Draw( void ) const; // TODO: clarify language between "draw", "render", etc

private:
	std::vector<SVertex3D> m_verVertices;
	std::vector<unsigned int> m_uiIndices;

	GLuint m_glVAO;
	GLuint m_glVBO;
	GLuint m_glEBO;
};

bool UTIL_Write( CFile *pFile, CGeometryResource *&pData );
bool UTIL_Read( CFile *pFile, CGeometryResource *&pData );
bool UTIL_GetValue( const CTextItem *pTextItem, CGeometryResource *&pValue );

#endif // GEOMETRYRESOURCE_H