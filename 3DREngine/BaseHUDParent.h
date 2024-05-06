#ifndef BASEHUDPARENT_H
#define BASEHUDPARENT_H

#include "Global.h"
#include "BaseHUDElement.h"

class CBaseHUDParent : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CBaseHUDParent, CBaseHUDElement )

	CBaseHUDParent();

	virtual bool GetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *&pMouseOver );
	virtual bool PropagateGetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *&pMouseOver );

	virtual void OnClick( void );
	virtual void OnRelease( void );

	virtual void OnGainFocus( void );
	virtual void OnLoseFocus( void );

	virtual void OnHover( void );
	virtual void OnUnhover( void );

private:
	CBaseHUDElement *m_pFocusedHUDElement;
	CBaseHUDElement *m_pHoveredHUDElement;
};

#endif // BASEHUDPARENT_H