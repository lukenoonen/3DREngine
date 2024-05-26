#include "TextInputLineHUDElement.h"

DEFINE_LINKED_CLASS( CTextInputLineHUDElement, hud_text_input_line )

CTextInputLineHUDElement::CTextInputLineHUDElement()
{

}

CTextInputLineHUDElement::~CTextInputLineHUDElement()
{
	while (!m_sEnteredText.empty())
		delete[] m_sEnteredText.front();
}

bool CTextInputLineHUDElement::HasEnteredText( void ) const
{
	return !m_sEnteredText.empty();
}

const char *CTextInputLineHUDElement::GetEnteredText( void ) const
{
	return m_sEnteredText.front();
}

void CTextInputLineHUDElement::NextEnteredText( void )
{
	delete[] m_sEnteredText.front();
	m_sEnteredText.pop();
}

void CTextInputLineHUDElement::ProcessInput( unsigned int uiChar, int iMods )
{
	switch (uiChar)
	{
	case GLFW_KEY_ENTER:
		m_sEnteredText.push( UTIL_strdup( GetText() ) );
		ClearText();
		return;
	}

	BaseClass::ProcessInput( uiChar, iMods );
}