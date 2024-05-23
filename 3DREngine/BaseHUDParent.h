#ifndef BASEHUDPARENT_H
#define BASEHUDPARENT_H

#include "Global.h"
#include "BaseHUDElement.h"

class CBaseHUDParent : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CBaseHUDParent, CBaseHUDElement )

	CBaseHUDParent();

	virtual void PostThink( void );

	virtual void SetHUDCamera( CBasePlayerCamera *pHUDCamera );
	virtual void PropagateSetHUDCamera( CBasePlayerCamera *pHUDCamera ) = 0;

	virtual CBaseHUDElement *GetMouseOver( EMouseOverType eMouseOverType );
	virtual CBaseHUDElement *PersonalGetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *pMouseOver );
	virtual CBaseHUDElement *PropagateGetMouseOver( EMouseOverType eMouseOverType ) = 0;

	virtual void PropagateClick( void );
	virtual void PropagateRelease( void );

	virtual void PropagateGainFocus( void );
	virtual void PropagateLoseFocus( void );

	virtual void PropagateHover( void );
	virtual void PropagateUnhover( void );

	virtual void ClearFocus( void );
	virtual void ClearHover( void );

	virtual bool FocusCheck( void );
	virtual bool HoverCheck( void );

private:
	CBaseHUDElement *m_pPrevFocusedHUDElement;
	CBaseHUDElement *m_pFocusedHUDElement;

	CBaseHUDElement *m_pPrevHoveredHUDElement;
	CBaseHUDElement *m_pHoveredHUDElement;
};

#endif // BASEHUDPARENT_H