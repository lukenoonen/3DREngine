#include "BaseHUDElement.h"
#include "RenderManager.h"

glm::vec3 UTIL_LocalToCamera( const glm::vec3 &vec3Local, const glm::mat4 &matTotal )
{
	glm::vec4 vec4Camera = matTotal * glm::vec4( vec3Local.x, vec3Local.z, -vec3Local.y, 1.0f );
	return vec4Camera / vec4Camera.w;
}

glm::vec3 UTIL_LocalToCamera( const glm::vec2 &vec2Local, const glm::mat4 &matTotal )
{
	return UTIL_LocalToCamera( glm::vec3( vec2Local, 0.0f ), matTotal );
}

glm::vec3 UTIL_CameraToLocal( const glm::vec3 &vec3Camera, const glm::mat4 &matTotalInverse )
{
	glm::vec4 vec4Local = matTotalInverse * glm::vec4( vec3Camera, 1.0f );
	return glm::vec3( vec4Local.x, -vec4Local.z, vec4Local.y ) / vec4Local.w;
}

glm::vec3 UTIL_CameraToLocal( const glm::vec2 &vec2Camera, const glm::mat4 &matTotalInverse )
{
	return UTIL_CameraToLocal( glm::vec3( vec2Camera, 0.0f ), matTotalInverse );
}

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

CBaseHUDElement *CBaseHUDElement::GetMouseOver( EMouseOverType eMouseOverType )
{
	return IsMouseOver() ? this : NULL;
}

void CBaseHUDElement::Click( void )
{
	m_bClicked = true;
	OnClick();
}

void CBaseHUDElement::OnClick( void )
{
	std::cout << (GetName() ? GetName() : "NULL") << " clicked\n";
}

void CBaseHUDElement::PropagateClick( void )
{

}

void CBaseHUDElement::Release( void )
{
	m_bClicked = false;
	OnRelease();
}

void CBaseHUDElement::OnRelease( void )
{
	std::cout << (GetName() ? GetName() : "NULL") << " released\n";
}

void CBaseHUDElement::PropagateRelease( void )
{

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
	std::cout << (GetName() ? GetName() : "NULL") << " gain focus\n";
}

void CBaseHUDElement::PropagateGainFocus( void )
{

}

void CBaseHUDElement::LoseFocus( void )
{
	if (IsClicked())
		Release();

	m_bFocus = false;
	OnLoseFocus();
}

void CBaseHUDElement::OnLoseFocus( void )
{
	std::cout << (GetName() ? GetName() : "NULL") << " lose focus\n";
}

void CBaseHUDElement::PropagateLoseFocus( void )
{

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
	std::cout << (GetName() ? GetName() : "NULL") << " hover\n";
}

void CBaseHUDElement::PropagateHover( void )
{

}

void CBaseHUDElement::Unhover( void )
{
	m_bHover = false;
	OnUnhover();
}

void CBaseHUDElement::OnUnhover( void )
{
	std::cout << (GetName() ? GetName() : "NULL") << " unhover\n";
}

void CBaseHUDElement::PropagateUnhover( void )
{

}

bool CBaseHUDElement::HasHover( void ) const
{
	return m_bHover;
}

void CBaseHUDElement::ClearFocus( void )
{
	if (HasFocus())
		LoseFocus();
}

void CBaseHUDElement::ClearHover( void )
{
	if (HasHover())
		Unhover();
}

bool CBaseHUDElement::FocusCheck( void )
{
	return HasFocus();
}

bool CBaseHUDElement::HoverCheck( void )
{
	return HasHover();
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
	glm::vec3 vec3Result = UTIL_LocalToCamera( glm::vec2( 0.0f, 0.0f ), matTotal );
	m_flDepth = vec3Result.z;
	m_vec2Bounds[0] = vec3Result;
	m_vec2Bounds[1] = UTIL_LocalToCamera( glm::vec2( 1.0f, 0.0f ), matTotal );
	m_vec2Bounds[2] = UTIL_LocalToCamera( glm::vec2( 1.0f, 1.0f ), matTotal );
	m_vec2Bounds[3] = UTIL_LocalToCamera( glm::vec2( 0.0f, 1.0f ), matTotal );

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