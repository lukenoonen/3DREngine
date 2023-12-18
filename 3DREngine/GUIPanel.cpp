#include "GUIPanel.h"
#include "RenderManager.h"

DEFINE_DATADESC( CGUIPanel )

	DEFINE_FIELD( LinkedDataField, CHandle<CGUIPanelMaterial>, m_hGUIPanelMaterial, "guipanelmaterial", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CGUIPanel, panel_gui )

CGUIPanel::CGUIPanel()
{

}

CGUIPanel::~CGUIPanel()
{
	glDeleteVertexArrays( 1, &m_glVAO );
	glDeleteBuffers( 1, &m_glVBO );
}

bool CGUIPanel::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (!m_hGUIPanelMaterial)
		return false;

	float vertices[] = {
		// pos      // tex
		0.0f, 0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 1.0f, 1.0f,

		1.0f, -1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays( 1, &m_glVAO );
	glGenBuffers( 1, &m_glVBO );

	glBindVertexArray( m_glVAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_glVBO );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void *)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof( float ), (void *)(2 * sizeof( float )));

	return true;
}

void CGUIPanel::Draw( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 0, &GetModelMatrix() ); // TODO: change "model" matrix to better name

	m_hGUIPanelMaterial->Apply();

	pRenderManager->SetUniform( "u_vecTranslation", GetPosition() );

	glBindVertexArray( m_glVAO );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
}

#include <TimeManager.h>

void CGUIPanel::PostThink( void )
{
	AddRotation( glm::vec3( 1.0f, 1.0f, 1.0f ) * pTimeManager->GetFrameTime() );

	BaseClass::PostThink();
}

bool CGUIPanel::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_hGUIPanelMaterial->ShouldApply();
}