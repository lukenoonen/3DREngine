#include "BaseHUDPanel.h"
#include "EntityManager.h"

DEFINE_DATADESC( CBaseHUDPanel )

	DEFINE_FIELD( LinkedDataField, CHandle<CTextInputHUDElement>, m_hTextInput, "textinput", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CBaseHUDPanel, hud_panel )

CBaseHUDPanel::CBaseHUDPanel()
{
	m_hDraggable = new CDraggableHUDElement();
	pEntityManager->AddEntity( m_hDraggable );
}

CBaseHUDPanel::~CBaseHUDPanel()
{
	m_hDraggable->Remove();
}

bool CBaseHUDPanel::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_hTextInput->SetPosition( g_vec3Zero );
	m_hTextInput->SetRotation( g_vec3Zero );
	m_hTextInput->SetScale( g_vec3One );
	m_hTextInput->SetParentRelative( this );

	m_hDraggable->SetParent( m_hParent );
	m_hDraggable->SetPosition( GetPosition() );
	m_hDraggable->SetRotation( GetRotation() );
	m_hDraggable->SetScale( GetScale() * glm::vec3( 1.0f, 1.0f, 0.05f ) );
	AddPosition( glm::vec3( 0.0f, 0.0f, -m_hDraggable->GetScale().z ) );
	SetParent( m_hDraggable );
	return true;
}


void CBaseHUDPanel::PropagateSetHUDCamera( CBasePlayerCamera *pHUDCamera )
{
	m_hDraggable->SetHUDCamera( pHUDCamera );
	m_hTextInput->SetHUDCamera( pHUDCamera );
}

CBaseHUDElement *CBaseHUDPanel::PropagateGetMouseOver( EMouseOverType eMouseOverType )
{
	CBaseHUDElement *pMouseOver;
	pMouseOver = m_hDraggable->GetMouseOver( eMouseOverType );
	if (pMouseOver)
		return pMouseOver;
	pMouseOver = m_hTextInput->GetMouseOver( eMouseOverType );
	if (pMouseOver)
		return pMouseOver;

	return NULL;
}