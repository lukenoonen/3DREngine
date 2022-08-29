#include "Geometry.h"

CGeometry::CGeometry( const std::vector<SVertex> &verVertices, const std::vector<unsigned int> &uiIndices, const char *sPath ) : BaseClass( sPath )
{
	m_verVertices = verVertices;
	m_uiIndices = uiIndices;

	glGenVertexArrays( 1, &m_uiVAO );
	glGenBuffers( 1, &m_uiVBO );
	glGenBuffers( 1, &m_uiEBO );

	glBindVertexArray( m_uiVAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_uiVBO );
	glBufferData( GL_ARRAY_BUFFER, m_verVertices.size() * sizeof( SVertex ), &m_verVertices[0], GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( SVertex ), (void *)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, sizeof( SVertex ), (void *)offsetof( SVertex, vec3Normal ) );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 3, GL_FLOAT, GL_FALSE, sizeof( SVertex ), (void *)offsetof( SVertex, vec3Tangent ) );
	glEnableVertexAttribArray( 3 );
	glVertexAttribPointer( 3, 3, GL_FLOAT, GL_FALSE, sizeof( SVertex ), (void *)offsetof( SVertex, vec3Bitangent ) );
	glEnableVertexAttribArray( 4 );
	glVertexAttribPointer( 4, 2, GL_FLOAT, GL_FALSE, sizeof( SVertex ), (void *)offsetof( SVertex, vec2TexCoords ) );
	glEnableVertexAttribArray( 5 );
	glVertexAttribIPointer( 5, 4, GL_INT, sizeof( SVertex ), (void *)offsetof( SVertex, vec4BoneIDs ) );
	glEnableVertexAttribArray( 6 );
	glVertexAttribPointer( 6, 4, GL_FLOAT, GL_FALSE, sizeof( SVertex ), (void *)offsetof( SVertex, vec4Weights ) );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_uiEBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_uiIndices.size() * sizeof( unsigned int ), &m_uiIndices[0], GL_STATIC_DRAW );
}

CGeometry::~CGeometry()
{
	glDeleteVertexArrays( 1, &m_uiVAO );
	glDeleteBuffers( 1, &m_uiVBO );
	glDeleteBuffers( 1, &m_uiEBO );
}

EAssetType CGeometry::GetAssetType( void ) const
{
	return EAssetType::t_geometry;
}

void CGeometry::Draw( void )
{
	glBindVertexArray( m_uiVAO );
	glDrawElements( GL_TRIANGLES, (GLsizei)m_uiIndices.size(), GL_UNSIGNED_INT, 0 );
}