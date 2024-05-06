#include "BaseHUDParent.h"

CBaseHUDParent::CBaseHUDParent()
{
	m_pFocusedHUDElement = NULL;
	m_pHoveredHUDElement = NULL;
}

bool CBaseHUDParent::GetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *&pMouseOver )
{
	bool bNewMouseOver = PropagateGetMouseOver( eMouseOverType, pMouseOver );

	switch (eMouseOverType)
	{
	case EMouseOverType::t_focus:
		if (m_pFocusedHUDElement != pMouseOver)
		{
			bNewMouseOver = true;
			m_pFocusedHUDElement = pMouseOver;
		}
		break;
	case EMouseOverType::t_hover:
		if (m_pHoveredHUDElement != pMouseOver)
		{
			bNewMouseOver = true;
			m_pHoveredHUDElement = pMouseOver;
		}
		break;
	}

	return BaseClass::GetMouseOver( eMouseOverType );
}

bool CBaseHUDParent::PropagateGetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *&pMouseOver )
{
	return false;
}

void CBaseHUDParent::OnClick( void )
{
	if (m_pFocusedHUDElement)
		m_pFocusedHUDElement->Click();
}

void CBaseHUDParent::OnRelease( void )
{
	if (m_pFocusedHUDElement)
		m_pFocusedHUDElement->Release();
}

void CBaseHUDParent::OnGainFocus( void )
{
	if (m_pFocusedHUDElement)
		m_pFocusedHUDElement->GainFocus();
}

void CBaseHUDParent::OnLoseFocus( void )
{
	if (m_pFocusedHUDElement)
		m_pFocusedHUDElement->LoseFocus();
}

void CBaseHUDParent::OnHover( void )
{
	if (m_pHoveredHUDElement)
		m_pHoveredHUDElement->Hover();
}

void CBaseHUDParent::OnUnhover( void )
{
	if (m_pHoveredHUDElement)
		m_pHoveredHUDElement->Unhover();
}