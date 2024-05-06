#include "BaseHUDElement.h"

DEFINE_LINKED_CLASS( CBaseHUDElement, hud_test )

CBaseHUDElement::CBaseHUDElement()
{
	m_pHUDCamera = NULL;
	m_bClicked = false;
	m_bFocus = false;
	m_bHover = false;
}

#include <iostream>
#include "InputManager.h"

void CBaseHUDElement::PreRender( void )
{
	BaseClass::PreRender();

	if (ModelMatrixUpdated() || true /* TODO: CAMERA CONDITION! */)
		UpdateBounds();
}

void CBaseHUDElement::SetHUDCamera( CBasePlayerCamera *pHUDCamera )
{
	m_pHUDCamera = pHUDCamera;
	UpdateBounds();
}

bool CBaseHUDElement::GetMouseOver( EMouseOverType eMouseOverType, CBaseHUDElement *&pMouseOver )
{
	if (!IsMouseOver())
	{
		pMouseOver = NULL;
		return false;
	}

	pMouseOver = this;
	return true;
}

void CBaseHUDElement::Click( void )
{
	m_bClicked = true;
	OnClick();
}

void CBaseHUDElement::OnClick( void )
{
	std::cout << GetName() << " clicked\n";
}

void CBaseHUDElement::Release( void )
{
	m_bClicked = false;
	OnRelease();
}

void CBaseHUDElement::OnRelease( void )
{
	std::cout << GetName() << " released\n";
}

bool CBaseHUDElement::IsClicked( void ) const
{
	return m_bClicked;
}

void CBaseHUDElement::GainFocus( void )
{
	m_bFocus = true;
	OnGainFocus();
}

void CBaseHUDElement::OnGainFocus( void )
{
	std::cout << GetName() << " gain focus\n";
}

void CBaseHUDElement::LoseFocus( void )
{
	m_bFocus = false;
	OnLoseFocus();
}

void CBaseHUDElement::OnLoseFocus( void )
{
	std::cout << GetName() << " lose focus\n";
}

bool CBaseHUDElement::HasFocus( void ) const
{
	return m_bFocus;
}

void CBaseHUDElement::Hover( void )
{
	m_bHover = true;
	OnHover();
}

void CBaseHUDElement::OnHover( void )
{
	std::cout << GetName() << " hover\n";
}

void CBaseHUDElement::Unhover( void )
{
	m_bHover = false;
	OnUnhover();
}

void CBaseHUDElement::OnUnhover( void )
{
	std::cout << GetName() << " unhover\n";
}

bool CBaseHUDElement::HasHover( void ) const
{
	return m_bHover;
}

CBasePlayerCamera *CBaseHUDElement::GetHUDCamera( void ) const
{
	return m_pHUDCamera;
}

float CBaseHUDElement::GetDepth( void ) const
{
	return m_flDepth;
}

void CBaseHUDElement::UpdateBounds( void )
{
	glm::mat4 matTotal = m_pHUDCamera->GetTotal() * GetModelMatrix();
	glm::vec4 vec4Result;
	vec4Result = matTotal * glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	vec4Result /= vec4Result.w;
	m_flDepth = vec4Result.z;
	m_vec2Bounds[0] = vec4Result;
	vec4Result = matTotal * glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f );
	m_vec2Bounds[1] = vec4Result / vec4Result.w;
	vec4Result = matTotal * glm::vec4( 1.0f, 0.0f, -1.0f, 1.0f );
	m_vec2Bounds[2] = vec4Result / vec4Result.w;
	vec4Result = matTotal * glm::vec4( 0.0f, 0.0f, -1.0f, 1.0f );
	m_vec2Bounds[3] = vec4Result / vec4Result.w;

	m_vec2Min = m_vec2Bounds[0];
	m_vec2Max = m_vec2Bounds[0];
	for (unsigned int i = 1; i < 4; i++)
	{
		if (m_vec2Min.x > m_vec2Bounds[i].x)
			m_vec2Min.x = m_vec2Bounds[i].x;
		else if (m_vec2Max.x < m_vec2Bounds[i].x)
			m_vec2Max.x = m_vec2Bounds[i].x;

		if (m_vec2Min.y > m_vec2Bounds[i].y)
			m_vec2Min.y = m_vec2Bounds[i].y;
		else if (m_vec2Max.y < m_vec2Bounds[i].y)
			m_vec2Max.y = m_vec2Bounds[i].y;
	}
}

bool CBaseHUDElement::IsMouseOver( void ) const
{
	const glm::vec2 &vec2CursorPosition = pInputManager->GetNormalizedCursorPosition();
	if (m_vec2Min.x <= vec2CursorPosition.x && m_vec2Min.y <= vec2CursorPosition.y
		&& m_vec2Max.x >= vec2CursorPosition.x && m_vec2Max.y >= vec2CursorPosition.y) // TODO: Add actual check instead of just AABB
		return true;
	return false;
}