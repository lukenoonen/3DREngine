#include "BaseHUDParent.h"
#include "InputManager.h"

CBaseHUDParent::CBaseHUDParent()
{
	m_pPrevFocusedHUDElement = NULL;
	m_hFocusedHUDElement = NULL;

	m_pPrevHoveredHUDElement = NULL;
	m_hHoveredHUDElement = NULL;
}

void CBaseHUDParent::PreThink( void )
{
	if (m_hFocusedHUDElement && !m_hFocusedHUDElement.Check())
		m_pPrevFocusedHUDElement = NULL;

	if (m_hHoveredHUDElement && !m_hHoveredHUDElement.Check())
		m_pPrevHoveredHUDElement = NULL;

	BaseClass::PreThink();
}

void CBaseHUDParent::CleanUp( void )
{
	m_hFocusedHUDElement.Check();
	m_hHoveredHUDElement.Check();

	BaseClass::CleanUp();
}

void CBaseHUDParent::SetHUDCamera( CBasePlayerCamera *pHUDCamera )
{
	BaseClass::SetHUDCamera( pHUDCamera );
	PropagateSetHUDCamera( pHUDCamera );
}

CBaseHUDElement *CBaseHUDParent::GetMouseOver( EMouseOverType eMouseOverType)
{
	CBaseHUDElement *pMouseOver = PropagateGetMouseOver( eMouseOverType );

	switch (eMouseOverType)
	{
	case EMouseOverType::t_focus:
		if (m_pPrevFocusedHUDElement != m_hFocusedHUDElement)
			m_pPrevFocusedHUDElement = m_hFocusedHUDElement;
		if (m_hFocusedHUDElement != pMouseOver)
			m_hFocusedHUDElement = pMouseOver;
		break;
	case EMouseOverType::t_hover:
		if (m_pPrevHoveredHUDElement != m_hHoveredHUDElement)
			m_pPrevHoveredHUDElement = m_hHoveredHUDElement;
		if (m_hHoveredHUDElement != pMouseOver)
			m_hHoveredHUDElement = pMouseOver;
		break;
	}

	return PersonalGetMouseOver( eMouseOverType, pMouseOver );
}

CBaseHUDElement *CBaseHUDParent::PersonalGetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *pMouseOver )
{
	return pMouseOver ? this : BaseClass::GetMouseOver( eMouseOverType );
}

void CBaseHUDParent::PropagateClick( void )
{
	if (m_hFocusedHUDElement)
	{
		m_hFocusedHUDElement->PropagateClick();
		m_hFocusedHUDElement->Click();
	}
}

void CBaseHUDParent::PropagateRelease( void )
{
	if (m_hFocusedHUDElement)
	{
		m_hFocusedHUDElement->PropagateRelease();
		m_hFocusedHUDElement->Release();
	}
}

void CBaseHUDParent::PropagateGainFocus( void )
{
	if (m_hFocusedHUDElement)
	{
		m_hFocusedHUDElement->PropagateGainFocus();
		if (m_hFocusedHUDElement != m_pPrevFocusedHUDElement)
			m_hFocusedHUDElement->GainFocus();
	}
}

void CBaseHUDParent::PropagateLoseFocus( void )
{
	if (m_pPrevFocusedHUDElement)
	{
		m_pPrevFocusedHUDElement->PropagateLoseFocus();
		if (m_pPrevFocusedHUDElement != m_hFocusedHUDElement)
			m_pPrevFocusedHUDElement->LoseFocus();
	}
}

void CBaseHUDParent::PropagateHover( void )
{
	if (m_hHoveredHUDElement)
	{
		m_hHoveredHUDElement->PropagateHover();
		if (m_hHoveredHUDElement != m_pPrevHoveredHUDElement)
			m_hHoveredHUDElement->Hover();
	}
}

void CBaseHUDParent::PropagateUnhover( void )
{
	if (m_pPrevHoveredHUDElement)
	{
		m_pPrevHoveredHUDElement->PropagateUnhover();
		if (m_pPrevHoveredHUDElement != m_hHoveredHUDElement)
			m_pPrevHoveredHUDElement->Unhover();
	}
}

void CBaseHUDParent::ClearFocus( void )
{
	if (m_hFocusedHUDElement)
	{
		m_hFocusedHUDElement->ClearFocus();
		m_hFocusedHUDElement = NULL;
		m_pPrevFocusedHUDElement = NULL;
		LoseFocus();
	}
	BaseClass::ClearFocus();
}

void CBaseHUDParent::ClearHover( void )
{
	if (m_hHoveredHUDElement)
	{
		m_hHoveredHUDElement->ClearHover();
		m_hHoveredHUDElement = NULL;
		m_pPrevHoveredHUDElement = NULL;
	}
	BaseClass::ClearHover();
}

bool CBaseHUDParent::FocusCheck( void )
{
	if (m_hFocusedHUDElement && !m_hFocusedHUDElement->FocusCheck())
	{
		m_hFocusedHUDElement = NULL;
		m_pPrevFocusedHUDElement = NULL;
		LoseFocus();
		return false;
	}
	return BaseClass::FocusCheck();
}

bool CBaseHUDParent::HoverCheck( void )
{
	if (m_hHoveredHUDElement && !m_hHoveredHUDElement->HoverCheck())
	{
		m_hHoveredHUDElement = NULL;
		m_pPrevHoveredHUDElement = NULL;
		Unhover();
		return false;
	}
	return BaseClass::HoverCheck();
}