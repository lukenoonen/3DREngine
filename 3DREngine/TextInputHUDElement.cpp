#include "TextInputHUDElement.h"
#include "InputManager.h"

DEFINE_DATADESC( CTextInputHUDElement )

	DEFINE_FIELD( LinkedDataField, CHandle<CGUIText>, m_hGUIText, "guitext", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CTextInputHUDElement, hud_text_input )

CTextInputHUDElement::CTextInputHUDElement()
{

}

#include <iostream>

void CTextInputHUDElement::Think( void )
{
	if (HasFocus())
	{
		for (unsigned int i = 0; i < pInputManager->GetTextCount(); i++)
		{
			unsigned int uiChar = pInputManager->GetText( i );
			switch (uiChar)
			{
			case GLFW_KEY_ESCAPE:
				LoseFocus();
				break;
			case GLFW_KEY_DELETE:
				m_uiPosition = m_hGUIText->Delete( m_uiPosition );
				break;
			case GLFW_KEY_BACKSPACE:
				m_uiPosition = m_hGUIText->Backspace( m_uiPosition );
				break;
			case GLFW_KEY_ENTER:
				m_uiPosition = m_hGUIText->Insert( m_uiPosition, '\n' );
				break;
			case GLFW_KEY_TAB:
				m_uiPosition = m_hGUIText->Insert( m_uiPosition, '\t' );
				break;
			case GLFW_KEY_RIGHT:
				m_uiPosition = m_hGUIText->GetRight( m_uiPosition );
				break;
			case GLFW_KEY_LEFT:
				m_uiPosition = m_hGUIText->GetLeft( m_uiPosition );
				break;
			case GLFW_KEY_DOWN:
				m_uiPosition = m_hGUIText->GetDown( m_uiPosition );
				break;
			case GLFW_KEY_UP:
				m_uiPosition = m_hGUIText->GetUp( m_uiPosition );
				break;
			default:
				m_uiPosition = m_hGUIText->Insert( m_uiPosition, (char)uiChar );
				break;
			}
		}
	}

	BaseClass::Think();
}

void CTextInputHUDElement::OnGainFocus( void )
{
	pInputManager->UnlockKeyboard();
	m_uiPosition = m_hGUIText->GetTextLength();
}

void CTextInputHUDElement::OnLoseFocus( void )
{
	pInputManager->LockKeyboard();
}

void CTextInputHUDElement::OnHover( void )
{
	pInputManager->SetCursor( ECursorShape::t_ibeam );
}

void CTextInputHUDElement::OnUnhover( void )
{
	pInputManager->ResetCursor();
}