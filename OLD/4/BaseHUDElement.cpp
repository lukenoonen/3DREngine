#include "BaseHUDElement.h"
#include "ShaderManager.h"

CBaseHUDElement::CBaseHUDElement( const glm::vec2 &vecPosition, const glm::vec2 &vecScale, float flPriority, bool bShouldDraw, bool bActive ) : BaseClass( vecPosition, vecScale, flPriority, bShouldDraw, bActive )
{
	float flVerticies[] =
	{
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,
	};

	unsigned int uiIndicies[] =
	{
		3, 1, 0,
		3, 2, 1,
	};

	glGenVertexArrays( 1, &m_uiVAO );
	glGenBuffers( 1, &m_uiVBO );
	glGenBuffers( 1, &m_uiEBO );

	glBindVertexArray( m_uiVAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_uiVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( float ) * 16, &flVerticies[0], GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 4, (void*)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 4, (void*)(sizeof( float ) * 2) );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_uiEBO );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof( unsigned int ), &uiIndicies[0], GL_STATIC_DRAW );
}

void CBaseHUDElement::Draw( void )
{
	float flPriority = GetPriority();

	pShaderManager->SetUniformBufferObject( UBO_HUD, 0, &GetPosition() );
	pShaderManager->SetUniformBufferObject( UBO_HUD, 1, &GetScale() );
	pShaderManager->SetUniformBufferObject( UBO_HUD, 2, &flPriority );

	glBindVertexArray( m_uiVAO );
	glDrawElements( GL_TRIANGLES, (GLsizei)6, GL_UNSIGNED_INT, 0 );
}