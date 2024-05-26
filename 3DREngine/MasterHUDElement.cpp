#include "MasterHUDElement.h"
#include "InputManager.h"

DEFINE_DATADESC( CMasterHUDElement )

	DEFINE_FIELD( LinkedDataField, CHandle<CBasePlayerCamera>, m_hHUDCamera, "hudcamera", FL_REQUIRED )
	DEFINE_FIELD( LinkedVectorDataField, CHandle<CBaseHUDElement>, m_hHUDChildren, "hudchildren", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CMasterHUDElement, hud_master )

CMasterHUDElement::CMasterHUDElement()
{

}

bool CMasterHUDElement::Init( void )
{
	if (!BaseClass::Init())
		return false;

	SetHUDCamera( m_hHUDCamera );

	return true;
}

#include <iostream>
#include "TimeManager.h"

void CMasterHUDElement::PreThink( void )
{
	for (unsigned int i = 0; i < m_hHUDChildren.size(); i++)
	{
		if (!m_hHUDChildren[i].Check())
			m_hHUDChildren.erase( m_hHUDChildren.begin() + i-- );
	}

	if (pInputManager->IsCursorLocked())
	{
		ClearHover();
		ClearFocus();
	}
	else
	{
		HoverCheck();
		FocusCheck();

		if (pInputManager->IsKeyPressed( EKeyCodes::t_m0 ))
		{
			CBaseHUDElement *pMouseOver = GetMouseOver( EMouseOverType::t_focus );
			if (pMouseOver)
			{
				auto it = std::find( m_hHUDChildren.begin(), m_hHUDChildren.end(), pMouseOver );
				if (it != m_hHUDChildren.begin() && it != m_hHUDChildren.end())
					std::rotate( m_hHUDChildren.begin(), it, it + 1 );
			}
			PropagateLoseFocus();
			PropagateGainFocus();
			PropagateClick();
		}
		else if (pInputManager->IsKeyReleased( EKeyCodes::t_m0 ))
		{
			PropagateRelease();
		}
		else if (!pInputManager->IsKeyDown( EKeyCodes::t_m0 ))
		{
			GetMouseOver( EMouseOverType::t_hover );
			PropagateUnhover();
			PropagateHover();
		}
	}

	BaseClass::PreThink();
}

void CMasterHUDElement::PropagateSetHUDCamera( CBasePlayerCamera *pHUDCamera )
{
	for (unsigned int i = 0; i < m_hHUDChildren.size(); i++)
		m_hHUDChildren[i]->SetHUDCamera( pHUDCamera );
}

CBaseHUDElement *CMasterHUDElement::PersonalGetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *pMouseOver )
{
	return pMouseOver;
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