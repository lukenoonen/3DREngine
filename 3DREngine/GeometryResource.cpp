#include "GeometryResource.h"
#include "ResourceManager.h"

CGeometryResource::CGeometryResource( SGeometryResourceData &sData, const char *sName ) : BaseClass( sName )
{
	m_verVertices = sData.verVertices;
	m_uiIndices = sData.uiIndices;

	glGenVertexArrays( 1, &m_glVAO );
	glGenBuffers( 1, &m_glVBO );
	glGenBuffers( 1, &m_glEBO );

	glBindVertexArray( m_glVAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_glVBO );
	glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)(m_verVertices.size() * sizeof( SVertex3D )), &m_verVertices[0], GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( SVertex3D ), (void *)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( SVertex3D ), (void *)offsetof( SVertex3D, vec3Normal ) );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof( SVertex3D ), (void *)offsetof( SVertex3D, vec3Tangent ) );
	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( SVertex3D ), (void *)offsetof( SVertex3D, vec3Bitangent ) );
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 4, 2, GL_FLOAT, GL_FALSE, sizeof( SVertex3D ), (void *)offsetof( SVertex3D, vec2TexCoords ) );
	glEnableVertexAttribArray( 5 );
	glVertexAttribIPointer( 5, 4, GL_INT, sizeof( SVertex3D ), (void *)offsetof( SVertex3D, vec4BoneIDs ) );
	glEnableVertexAttribArray( 6 );
	glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( SVertex3D ), (void *)offsetof( SVertex3D, vec4Weights ) );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_glEBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(m_uiIndices.size() * sizeof( unsigned int )), &m_uiIndices[0], GL_STATIC_DRAW );
}

CGeometryResource::~CGeometryResource()
{
	// TODO: See if checks for == 0 should be put back
	glDeleteVertexArrays( 1, &m_glVAO );
	glDeleteBuffers( 1, &m_glVBO );
	glDeleteBuffers( 1, &m_glEBO );
}

void CGeometryResource::Draw( void ) const
{
	glBindVertexArray( m_glVAO );
	glDrawElements( GL_TRIANGLES, (GLsizei)m_uiIndices.size(), GL_UNSIGNED_INT, 0 );
}

bool UTIL_Write( CFile *pFile, CGeometryResource *&pData )
{
	const char *sName = pData->GetName();
	return pFile->Write( sName );
}

bool UTIL_Read( CFile *pFile, CGeometryResource *&pData )
{
	char *sName;
	if (!pFile->Read( sName ))
		return false;

	pData = pResourceManager->GetGeometryResource( sName );
	delete[] sName;
	return pData != NULL;
}

bool UTIL_GetValue( const CTextItem *pTextItem, CGeometryResource *&pValue )
{
	const char *sName;
	if (!pTextItem->GetValue( sName ))
		return false;

	pValue = pResourceManager->GetGeometryResource( sName );
	return pValue != NULL;
}