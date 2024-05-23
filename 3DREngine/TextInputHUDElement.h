#ifndef TEXTINPUTHUDELEMENT_H
#define TEXTINPUTHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"
#include "GUIText.h"
#include "GUICursor.h"

class CTextInputHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CTextInputHUDElement, CBaseHUDElement )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CTextInputHUDElement();

	virtual bool Init( void );

	virtual void Think( void );

	virtual void OnClick( void );
	virtual void OnRelease( void );

	virtual void OnGainFocus( void );
	virtual void OnLoseFocus( void );

	virtual void OnHover( void );
	virtual void OnUnhover( void );

private:
	glm::vec2 CalculateGUITextCursorPosition( void ) const;

private:
	CHandle<CGUIText> m_hGUIText;
	CHandle<CGUICursor> m_hGUICursor;
};

#endif // TEXTINPUTHUDELEMENT_H