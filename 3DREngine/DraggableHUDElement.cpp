#include "DraggableHUDElement.h"
#include "InputManager.h"

DEFINE_LINKED_CLASS( CDraggableHUDElement, hud_draggable )

CDraggableHUDElement::CDraggableHUDElement()
{

}

void CDraggableHUDElement::Think( void )
{
	if (IsClicked())
	{
		CBasePlayerCamera *pHUDCamera = GetHUDCamera();
		float flDepth = GetDepth();
		glm::mat4 matTotalInverse = glm::inverse( pHUDCamera->GetTotal() );
		glm::vec4 vec4From = glm::vec4( 0.0f, 0.0f, flDepth, 1.0f);
		glm::vec4 vec4To = glm::vec4( pInputManager->GetNormalizedCursorDelta(), flDepth, 1.0f );
		vec4From = matTotalInverse * vec4From;
		vec4From /= vec4From.w;
		vec4To = matTotalInverse * vec4To;
		vec4To /= vec4To.w;
		glm::vec3 vec3Delta = vec4To - vec4From;
		AddPosition( vec3Delta );
	}

	BaseClass::Think();
}

void CDraggableHUDElement::OnHover( void )
{
	pInputManager->SetCursor( ECursorShape::t_hand );
	BaseClass::OnHover();
}

void CDraggableHUDElement::OnUnhover( void )
{
	pInputManager->ResetCursor( ECursorShape::t_hand );
	BaseClass::OnUnhover();
}