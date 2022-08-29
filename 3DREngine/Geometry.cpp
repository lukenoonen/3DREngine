#include "Geometry.h"
#include "AssetManager.h"

CGeometry::CGeometry()
{
	m_glVAO = 0;
	m_glVBO = 0;
	m_glEBO = 0;
}

bool CGeometry::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (!pAssetManager->LoadGeometry( GetFilePath(), m_verVertices, m_uiIndices ))
		return false;


	glGenVertexArrays( 1, &m_glVAO );
	glGenBuffers( 1, &m_glVBO );
	glGenBuffers( 1, &m_glEBO );

	glBindVertexArray( m_glVAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_glVBO );
	glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)(m_verVertices.size() * sizeof( SVertex )), &m_verVertices[0], GL_STATIC_DRAW );

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

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_glEBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(m_uiIndices.size() * sizeof( unsigned int )), &m_uiIndices[0], GL_STATIC_DRAW );

	return true;
}

bool CGeometry::Exit( void )
{
	if (m_glVAO != 0)
		glDeleteVertexArrays( 1, &m_glVAO );

	if (m_glVBO != 0)
		glDeleteBuffers( 1, &m_glVBO );

	if (m_glEBO != 0)
		glDeleteBuffers( 1, &m_glEBO );

	return BaseClass::Exit();
}

void CGeometry::Draw( void )
{
	glBindVertexArray( m_glVAO );
	glDrawElements( GL_TRIANGLES, (GLsizei)m_uiIndices.size(), GL_UNSIGNED_INT, 0 );
}