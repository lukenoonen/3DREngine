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

void CMasterHUDElement::PreThink( void )
{
	std::vector<CHandle<CBaseHUDElement>>::iterator it = m_hHUDChildren.begin();
	while (it != m_hHUDChildren.end())
	{
		if (!it->Check())
		{
			it = m_hHUDChildren.erase( it );
			continue;
		}
		it++;
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
	std::vector<CHandle<CBaseHUDElement>>::iterator it;
	for (it = m_hHUDChildren.begin(); it != m_hHUDChildren.end(); it++)
		(*it)->SetHUDCamera( pHUDCamera );
}

CBaseHUDElement *CMasterHUDElement::PersonalGetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *pMouseOver )
{
	return pMouseOver;
}

CBaseHUDElement *CMasterHUDElement::PropagateGetMouseOver( EMouseOverType eMouseOverType )
{
	std::vector<CHandle<CBaseHUDElement>>::iterator it;
	for (it = m_hHUDChildren.begin(); it != m_hHUDChildren.end(); it++)
	{
		CBaseHUDElement *pMouseOver = (*it)->GetMouseOver( eMouseOverType );
		if (pMouseOver)
			return pMouseOver;
	}

	return NULL;
}