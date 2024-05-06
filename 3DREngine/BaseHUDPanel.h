#ifndef BASEHUDPANEL_H
#define BASEHUDPANEL_H

#include "Global.h"
#include "BaseHUDELement.h"
#include "DraggableHUDElement.h"

class CBaseHUDPanel : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CBaseHUDPanel, CBaseHUDElement )

	DECLARE_DATADESC()

	CBaseHUDPanel();

	virtual bool Init( void );

private:
	CHandle<CDraggableHUDElement> m_hDraggable;
};

#endif // BASEHUDPANEL_H