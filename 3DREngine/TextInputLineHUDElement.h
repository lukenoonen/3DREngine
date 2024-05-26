#ifndef TEXTINPUTLINEHUDELEMENT_H
#define TEXTINPUTLINEHUDELEMENT_H

#include "Global.h"
#include "TextInputHUDElement.h"

class CTextInputLineHUDElement : public CTextInputHUDElement
{
public:
	DECLARE_CLASS( CTextInputLineHUDElement, CTextInputHUDElement )

	DECLARE_LINKED_CLASS()

	CTextInputLineHUDElement();
	virtual ~CTextInputLineHUDElement();

	bool HasEnteredText( void ) const;
	const char *GetEnteredText( void ) const;
	void NextEnteredText( void );

protected:
	virtual void ProcessInput( unsigned int uiChar, int iMods );

private:
	std::queue<char *> m_sEnteredText;
};

#endif // TEXTINPUTLINEHUDELEMENT_H