#include "BaseHUDParent.h"
#include "InputManager.h"

CBaseHUDParent::CBaseHUDParent()
{
	m_pPrevHoveredHUDElement = NULL;
	m_pHoveredHUDElement = NULL;

	m_pPrevHoveredHUDElement = NULL;
	m_pHoveredHUDElement = NULL;
}

void CBaseHUDParent::PostThink( void )
{
	/*if (!pInputManager->IsCursorLocked())
	{
		if (m_pHoveredHUDElement && !m_pHoveredHUDElement->HasHover())
			m_pHoveredHUDElement = NULL;

		if (m_pFocusedHUDElement && !m_pFocusedHUDElement->HasFocus())
			m_pFocusedHUDElement = NULL;
	}
	else
	{
		if (m_pHoveredHUDElement)
		{
			if (m_pHoveredHUDElement->HasHover())
				m_pHoveredHUDElement->Unhover();

			m_pHoveredHUDElement = NULL;
		}

		if (m_pFocusedHUDElement)
		{
			if (m_pFocusedHUDElement->IsClicked())
				m_pFocusedHUDElement->Release();

			if (m_pFocusedHUDElement->HasFocus())
				m_pFocusedHUDElement->LoseFocus();

			m_pFocusedHUDElement = NULL;
		}
	}*/

	BaseClass::PostThink();
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
		m_pPrevFocusedHUDElement = m_pFocusedHUDElement;
		m_pFocusedHUDElement = pMouseOver;
		break;
	case EMouseOverType::t_hover:
		m_pPrevHoveredHUDElement = m_pHoveredHUDElement;
		m_pHoveredHUDElement = pMouseOver;
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
	if (m_pFocusedHUDElement)
	{
		m_pFocusedHUDElement->PropagateClick();
		m_pFocusedHUDElement->Click();
	}
}

void CBaseHUDParent::PropagateRelease( void )
{
	if (m_pFocusedHUDElement)
	{
		m_pFocusedHUDElement->PropagateRelease();
		m_pFocusedHUDElement->Release();
	}
}

void CBaseHUDParent::PropagateGainFocus( void )
{
	if (m_pFocusedHUDElement)
	{
		m_pFocusedHUDElement->PropagateGainFocus();
		if (m_pFocusedHUDElement != m_pPrevFocusedHUDElement)
			m_pFocusedHUDElement->GainFocus();
	}
}

void CBaseHUDParent::PropagateLoseFocus( void )
{
	if (m_pPrevFocusedHUDElement)
	{
		m_pPrevFocusedHUDElement->PropagateLoseFocus();
		if (m_pPrevFocusedHUDElement != m_pFocusedHUDElement)
			m_pPrevFocusedHUDElement->LoseFocus();
	}
}

void CBaseHUDParent::PropagateHover( void )
{
	if (m_pHoveredHUDElement)
	{
		m_pHoveredHUDElement->PropagateHover();
		if (m_pHoveredHUDElement != m_pPrevHoveredHUDElement)
			m_pHoveredHUDElement->Hover();
	}
}

void CBaseHUDParent::PropagateUnhover( void )
{
	if (m_pPrevHoveredHUDElement)
	{
		m_pPrevHoveredHUDElement->PropagateUnhover();
		if (m_pPrevHoveredHUDElement != m_pHoveredHUDElement)
			m_pPrevHoveredHUDElement->Unhover();
	}
}

void CBaseHUDParent::ClearFocus( void )
{
	if (m_pFocusedHUDElement)
	{
		m_pFocusedHUDElement->ClearFocus();
		m_pFocusedHUDElement = NULL;
		m_pPrevFocusedHUDElement = NULL;
		LoseFocus();
	}
	BaseClass::ClearFocus();
}

void CBaseHUDParent::ClearHover( void )
{
	if (m_pHoveredHUDElement)
	{
		m_pHoveredHUDElement->ClearHover();
		m_pHoveredHUDElement = NULL;
		m_pPrevHoveredHUDElement = NULL;
	}
	BaseClass::ClearHover();
}

bool CBaseHUDParent::FocusCheck( void )
{
	if (m_pFocusedHUDElement && !m_pFocusedHUDElement->FocusCheck())
	{
		m_pFocusedHUDElement = NULL;
		m_pPrevFocusedHUDElement = NULL;
		LoseFocus();
		return false;
	}
	return BaseClass::FocusCheck();
}

bool CBaseHUDParent::HoverCheck( void )
{
	if (m_pHoveredHUDElement && !m_pHoveredHUDElement->HoverCheck())
	{
		m_pHoveredHUDElement = NULL;
		m_pPrevHoveredHUDElement = NULL;
		Unhover();
		return false;
	}
	return BaseClass::HoverCheck();
}