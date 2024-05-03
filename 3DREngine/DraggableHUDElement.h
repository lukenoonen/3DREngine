#ifndef DRAGGABLEHUDELEMENT_H
#define DRAGGABLEHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"

class CDraggableHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CDraggableHUDElement, CBaseHUDElement )

	DECLARE_LINKED_CLASS()

	CDraggableHUDElement();

	virtual void Think( void );

	virtual void OnHover( void );
	virtual void OnUnhover( void );
};

#endif // DRAGGABLEHUDELEMENT_H