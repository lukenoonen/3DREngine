#include "BaseHUDPanel.h"
#include "EntityManager.h"
#include "KeyValues.h"

DEFINE_DATADESC( CBaseHUDPanel )

	DEFINE_FIELD( LinkedDataField, CHandle<CTextInputLineHUDElement>, m_hTextInputLine, "textinputline", FL_REQUIRED )
	DEFINE_FIELD( LinkedDataField, CHandle<CGUILog>, m_hGUILog, "guilog", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CBaseHUDPanel, hud_panel )

CBaseHUDPanel::CBaseHUDPanel()
{
	CKeyValues kvDraggableInit;
	m_hDraggable = pEntityManager->AddEntity<CDraggableHUDElement>( &kvDraggableInit );
}

bool CBaseHUDPanel::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_hTextInputLine->SetPosition( g_vec3Down * 0.1f );
	m_hTextInputLine->SetRotation( g_vec3Zero );
	m_hTextInputLine->SetScale( g_vec3One );
	m_hTextInputLine->SetParentRelative( this );

	m_hGUILog->SetPosition( g_vec3Zero );
	m_hGUILog->SetRotation( g_vec3Zero );
	m_hGUILog->SetScale( g_vec3One );
	m_hGUILog->SetParentRelative( this );

	m_hDraggable->SetParent( m_hParent );
	m_hDraggable->SetPosition( GetPosition() );
	m_hDraggable->SetRotation( GetRotation() );
	m_hDraggable->SetScale( GetScale() * glm::vec3( 1.0f, 1.0f, 0.05f ) );
	AddPosition( glm::vec3( 0.0f, 0.0f, -m_hDraggable->GetScale().z ) );
	SetParent( m_hDraggable );

	return true;
}

void CBaseHUDPanel::PostThink( void )
{
	while (m_hTextInputLine->HasEnteredText())
	{
		const char *sEnteredText = m_hTextInputLine->GetEnteredText();
		m_hGUILog->AddText( sEnteredText );
		m_hTextInputLine->NextEnteredText();
	}

	BaseClass::PostThink();
}

void CBaseHUDPanel::Remove( void )
{
	m_hDraggable->Remove();
	m_hDraggable.Check();
	
	m_hTextInputLine->Remove();
	m_hTextInputLine.Check();

	m_hGUILog->Remove();
	m_hGUILog.Check();

	BaseClass::Remove();
}

void CBaseHUDPanel::PropagateSetHUDCamera( CBasePlayerCamera *pHUDCamera )
{
	m_hDraggable->SetHUDCamera( pHUDCamera );
	m_hTextInputLine->SetHUDCamera( pHUDCamera );
}

CBaseHUDElement *CBaseHUDPanel::PropagateGetMouseOver( EMouseOverType eMouseOverType )
{
	CBaseHUDElement *pMouseOver;
	pMouseOver = m_hDraggable->GetMouseOver( eMouseOverType );
	if (pMouseOver)
		return pMouseOver;
	pMouseOver = m_hTextInputLine->GetMouseOver( eMouseOverType );
	if (pMouseOver)
		return pMouseOver;

	return NULL;
}