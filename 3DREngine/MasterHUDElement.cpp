#include "MasterHUDElement.h"
#include "InputManager.h"

DEFINE_DATADESC( CMasterHUDElement )

	DEFINE_FIELD( LinkedDataField, CHandle<CBasePlayerCamera>, m_hHUDCamera, "hudcamera", FL_REQUIRED )
	DEFINE_FIELD( LinkedVectorDataField, CHandle<CBaseHUDElement>, m_hHUDChildren, "hudchildren", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CMasterHUDElement, hud_master )

CMasterHUDElement::CMasterHUDElement()
{
	m_pFocusedHUDElement = NULL;
	m_pHoveredHUDElement = NULL;
}

bool CMasterHUDElement::Init( void )
{
	if (!BaseClass::Init())
		return false;

	SetHUDCamera( m_hHUDCamera );

	for (unsigned int i = 0; i < m_hHUDChildren.size(); i++)
		m_hHUDChildren[i]->SetHUDCamera( m_hHUDCamera );

	return true;
}

void CMasterHUDElement::PreThink( void )
{
	if (!pInputManager->IsCursorLocked())
	{
		if (m_pHoveredHUDElement && !m_pHoveredHUDElement->HasHover())
			m_pHoveredHUDElement = NULL;

		if (m_pFocusedHUDElement && !m_pFocusedHUDElement->HasFocus())
			m_pFocusedHUDElement = NULL;

		if (pInputManager->IsKeyPressed( EKeyCodes::t_m0 ))
		{
			unsigned int uiMouseOverIndex;
			CBaseHUDElement *pMouseOver = GetMouseOverIndex( uiMouseOverIndex );
			if (m_pFocusedHUDElement != pMouseOver)
			{
				if (m_pFocusedHUDElement)
					m_pFocusedHUDElement->LoseFocus();

				if (pMouseOver)
				{
					if (uiMouseOverIndex != 0)
					{
						auto it = m_hHUDChildren.begin();
						std::rotate( it, it + uiMouseOverIndex, it + uiMouseOverIndex + 1 );
					}
					pMouseOver->GainFocus();
					pMouseOver->Click();
				}

				m_pFocusedHUDElement = pMouseOver;
			}
			else
			{
				if (pMouseOver)
					pMouseOver->Click();
			}
		}
		else if (pInputManager->IsKeyReleased( EKeyCodes::t_m0 ))
		{
			if (m_pFocusedHUDElement)
				m_pFocusedHUDElement->Release();
		}
		else if (!pInputManager->IsKeyDown( EKeyCodes::t_m0 ))
		{
			CBaseHUDElement *pMouseOver = GetMouseOver( EMouseOverType::t_hover );
			if (m_pHoveredHUDElement != pMouseOver)
			{
				if (m_pHoveredHUDElement)
					m_pHoveredHUDElement->Unhover();

				if (pMouseOver)
					pMouseOver->Hover();

				m_pHoveredHUDElement = pMouseOver;
			}
		}
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

			m_pHoveredHUDElement = NULL;
		}
	}

	BaseClass::PreThink();
}

CBaseHUDElement *CMasterHUDElement::PropagateGetMouseOver( EMouseOverType eMouseOverType )
{
	for (unsigned int i = 0; i < m_hHUDChildren.size(); i++)
	{
		CBaseHUDElement *pMouseOver = m_hHUDChildren[i]->GetMouseOver( eMouseOverType );
		if (pMouseOver)
			return pMouseOver;
	}

	return NULL;
}

/*CBaseHUDElement *CMasterHUDElement::GetMouseOver(EMouseOverType eMouseOverType)
{
	for (unsigned int i = 0; i < m_hHUDChildren.size(); i++)
	{
		CBaseHUDElement *pMouseOver = m_hHUDChildren[i]->GetMouseOver();
		if (pMouseOver)
			return pMouseOver;
	}

	return NULL;
}

CBaseHUDElement *CMasterHUDElement::GetMouseOverIndex( unsigned int &uiMouseOverIndex )
{
	for (unsigned int i = 0; i < m_hHUDChildren.size(); i++)
	{
		CBaseHUDElement *pMouseOver = m_hHUDChildren[i]->GetMouseOver();
		if (pMouseOver)
		{
			uiMouseOverIndex = i;
			return pMouseOver;
		}
	}

	return NULL;
}*/