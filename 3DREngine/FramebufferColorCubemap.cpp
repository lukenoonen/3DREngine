#include "FramebufferColorCubemap.h"
#include "RenderManager.h"

CFramebufferColorCubemap::CFramebufferColorCubemap()
{

}

int CFramebufferColorCubemap::Bind( void ) const
{
	return pRenderManager->BindTexture( m_glTexture, GL_TEXTURE_CUBE_MAP );
}

void CFramebufferColorCubemap::CreateTextureBuffers( void )
{
	// TODO: implement this
}

void CFramebufferColorCubemap::CreateMSAABuffers( void )
{
	// TODO: implement this
}