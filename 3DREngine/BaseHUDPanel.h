#ifndef BASEHUDPANEL_H
#define BASEHUDPANEL_H

#include "Global.h"
#include "BaseHUDParent.h"
#include "DraggableHUDElement.h"
#include "TextInputLineHUDElement.h"
#include "GUILog.h"

class CBaseHUDPanel : public CBaseHUDParent
{
public:
	DECLARE_CLASS( CBaseHUDPanel, CBaseHUDParent )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CBaseHUDPanel();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Remove( void );

	virtual void PropagateSetHUDCamera( CBasePlayerCamera *pHUDCamera );

	virtual CBaseHUDElement *PropagateGetMouseOver( EMouseOverType eMouseOverType );

private:
	CHandle<CDraggableHUDElement> m_hDraggable;
	CHandle<CTextInputLineHUDElement> m_hTextInputLine;
	CHandle<CGUILog> m_hGUILog;
};

#endif // BASEHUDPANEL_H