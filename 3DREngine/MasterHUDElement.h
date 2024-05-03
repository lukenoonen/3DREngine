#ifndef MASTERHUDELEMENT_H
#define MASTERHUDELEMENT_H

#include "Global.h"
#include "BaseHUDElement.h"
#include "BasePlayerCamera.h"

class CMasterHUDElement : public CBaseHUDElement
{
public:
	DECLARE_CLASS( CMasterHUDElement, CBaseHUDElement )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CMasterHUDElement();

	virtual bool Init( void );

	virtual void PreThink( void );

	virtual CBaseHUDElement *GetMouseOver( void );

protected:
	CBaseHUDElement *GetMouseOverIndex( unsigned int &uiMouseOverIndex );

private:
	CHandle<CBasePlayerCamera> m_hHUDCamera;
	std::vector<CHandle<CBaseHUDElement>> m_hHUDChildren;

	CBaseHUDElement *m_pFocusedHUDElement;
	CBaseHUDElement *m_pHoveredHUDElement;
};

#endif // MASTERHUDELEMENT_H