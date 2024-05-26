#include "BasePlayerCamera.h"
#include "RenderManager.h"

CBasePlayerCamera::CBasePlayerCamera()
{

}

CBaseFramebuffer *CBasePlayerCamera::GetFramebuffer( void )
{
	return pRenderManager->GetDefaultFramebuffer();
}

const CBaseFramebuffer *CBasePlayerCamera::GetFramebuffer( void ) const
{
	return pRenderManager->GetDefaultFramebuffer();
}