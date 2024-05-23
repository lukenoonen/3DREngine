#ifndef BASEHUDPANEL_H
#define BASEHUDPANEL_H

#include "Global.h"
#include "BaseHUDParent.h"
#include "DraggableHUDElement.h"
#include "TextInputHUDElement.h"

class CBaseHUDPanel : public CBaseHUDParent
{
public:
	DECLARE_CLASS( CBaseHUDPanel, CBaseHUDParent )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CBaseHUDPanel();
	virtual ~CBaseHUDPanel();

	virtual bool Init( void );

	virtual void PropagateSetHUDCamera( CBasePlayerCamera *pHUDCamera );

	virtual CBaseHUDElement *PropagateGetMouseOver( EMouseOverType eMouseOverType );

private:
	CHandle<CDraggableHUDElement> m_hDraggable;
	CHandle<CTextInputHUDElement> m_hTextInput;
};

#endif // BASEHUDPANEL_H