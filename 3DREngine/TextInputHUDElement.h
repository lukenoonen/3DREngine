#ifndef TEXTINPUTHUDELEMENT_H
#define TEXTINPUTHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"
#include "GUIText.h"

class CTextInputHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CTextInputHUDElement, CBaseHUDElement )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CTextInputHUDElement();

	virtual void Think( void );

	virtual void OnGainFocus( void );
	virtual void OnLoseFocus( void );

	virtual void OnHover( void );
	virtual void OnUnhover( void );

private:
	CHandle<CGUIText> m_hGUIText;
	unsigned int m_uiPosition;
};

#endif // TEXTINPUTHUDELEMENT_H