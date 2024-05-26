#include "TextInputHUDElement.h"
#include "InputManager.h"

DEFINE_DATADESC( CTextInputHUDElement )

	DEFINE_FIELD( LinkedDataField, CHandle<CGUIText>, m_hGUIText, "guitext", FL_REQUIRED )
	DEFINE_FIELD( LinkedDataField, CHandle<CGUICursor>, m_hGUICursor, "guicursor", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CTextInputHUDElement, hud_text_input )

CTextInputHUDElement::CTextInputHUDElement()
{

}

bool CTextInputHUDElement::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_hGUIText->SetPosition( g_vec3Zero );
	m_hGUIText->SetRotation( g_vec3Zero );
	m_hGUIText->SetScale( g_vec3One );
	m_hGUIText->SetParentRelative( this );

	m_hGUICursor->SetGUIText( m_hGUIText );
	return true;
}

void CTextInputHUDElement::Think( void )
{
	CBasePlayerCamera *pHUDCamera = GetHUDCamera();
	if (HasFocus())
	{
		for (unsigned int i = 0; i < pInputManager->GetTextCount(); i++)
		{
			unsigned int uiChar = pInputManager->GetTextChar( i );
			int iMods = pInputManager->GetTextMods( i );
			ProcessInput( uiChar, iMods );
		}
	}

	if (IsClicked())
	{
		m_hGUICursor->Drag( CalculateGUITextCursorPosition() );
	}

	BaseClass::Think();
}

void CTextInputHUDElement::Remove( void )
{
	m_hGUIText->Remove();
	m_hGUIText.Check();

	m_hGUICursor->Remove();
	m_hGUICursor.Check();

	BaseClass::Remove();
}

void CTextInputHUDElement::OnClick( void )
{
	m_hGUICursor->Click( CalculateGUITextCursorPosition() );
	m_hGUICursor->StartBlinking();
	BaseClass::OnClick();
}

void CTextInputHUDElement::OnRelease( void )
{
	m_hGUICursor->Release( CalculateGUITextCursorPosition() );
	BaseClass::OnRelease();
}

void CTextInputHUDElement::OnGainFocus( void )
{
	pInputManager->UnlockKeyboard();
	BaseClass::OnGainFocus();
}

void CTextInputHUDElement::OnLoseFocus( void )
{
	pInputManager->LockKeyboard();
	m_hGUICursor->StopBlinking();
	BaseClass::OnLoseFocus();
}

void CTextInputHUDElement::OnHover( void )
{
	pInputManager->SetCursor( ECursorShape::t_ibeam );
	BaseClass::OnHover();
}

void CTextInputHUDElement::OnUnhover( void )
{
	pInputManager->ResetCursor( ECursorShape::t_ibeam );
	BaseClass::OnUnhover();
}

void CTextInputHUDElement::ClearText( void )
{
	m_hGUIText->SetText( "" );
	m_hGUICursor->MoveToStart();
}

const char *CTextInputHUDElement::GetText( void )
{
	return m_hGUIText->GetText();
}

void CTextInputHUDElement::ProcessInput( unsigned int uiChar, int iMods )
{
	switch (uiChar)
	{
	case GLFW_KEY_ESCAPE:
		LoseFocus();
		return;
	case GLFW_KEY_DELETE:
		m_hGUICursor->Delete();
		return;
	case GLFW_KEY_BACKSPACE:
		m_hGUICursor->Backspace();
		return;
	case GLFW_KEY_ENTER:
		m_hGUICursor->Insert( '\n' );
		return;
	case GLFW_KEY_TAB:
		m_hGUICursor->Insert( '\t' );
		return;
	case GLFW_KEY_RIGHT:
		m_hGUICursor->MoveHorizontal( 1 );
		return;
	case GLFW_KEY_LEFT:
		m_hGUICursor->MoveHorizontal( -1 );
		return;
	case GLFW_KEY_DOWN:
		m_hGUICursor->MoveVertical( 1 );
		return;
	case GLFW_KEY_UP:
		m_hGUICursor->MoveVertical( -1 );
		return;
	}

	char cChar = (char)uiChar;
	if (iMods & GLFW_MOD_CONTROL) // TODO: handle this better, newlines can be entered if in clipboard string!
	{
		switch (cChar)
		{
		case 'V':
			m_hGUICursor->Insert( pInputManager->GetClipboardString() );
			return;
		}
		return;
	}

	m_hGUICursor->Insert( cChar );
}

glm::vec2 CTextInputHUDElement::CalculateGUITextCursorPosition( void ) const
{
	CBasePlayerCamera *pHUDCamera = GetHUDCamera();
	glm::mat4 matTotalInverse = glm::inverse( pHUDCamera->GetTotal() * m_hGUIText->GetModelMatrix() );
	return UTIL_CameraToLocal( pInputManager->GetNormalizedCursorPosition(), matTotalInverse );
}