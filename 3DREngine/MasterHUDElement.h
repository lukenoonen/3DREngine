#ifndef MASTERHUDELEMENT_H
#define MASTERHUDELEMENT_H

#include "Global.h"
#include "BaseHUDParent.h"
#include "BaseHUDElement.h"
#include "BasePlayerCamera.h"

class CMasterHUDElement : public CBaseHUDParent
{
public:
	DECLARE_CLASS( CMasterHUDElement, CBaseHUDParent )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CMasterHUDElement();

	virtual bool Init( void );

	virtual void PreThink( void );

	virtual void PropagateSetHUDCamera( CBasePlayerCamera *pHUDCamera );

	virtual CBaseHUDElement *PersonalGetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *pMouseOver );
	virtual CBaseHUDElement *PropagateGetMouseOver( EMouseOverType eMouseOverType );

private:
	CHandle<CBasePlayerCamera> m_hHUDCamera;
	std::vector<CHandle<CBaseHUDElement>> m_hHUDChildren;
};

#endif // MASTERHUDELEMENT_H