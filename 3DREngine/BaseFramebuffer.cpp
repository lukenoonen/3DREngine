#include "BaseFramebuffer.h"

CBaseFramebuffer::CBaseFramebuffer()
{

}

int CBaseFramebuffer::Bind( void )
{
	return -1;
}

const glm::ivec2 &CBaseFramebuffer::GetSize( void ) const
{
	return g_ivec2Zero;
}

GLuint CBaseFramebuffer::GetFramebuffer( void ) const
{
	return 0;
}

void CBaseFramebuffer::Blit( void )
{

}

void CBaseFramebuffer::CreateTextureBuffers( void )
{

}

void CBaseFramebuffer::DestroyTextureBuffers( void )
{

}

void CBaseFramebuffer::CreateMSAABuffers( void )
{

}

void CBaseFramebuffer::DestroyMSAABuffers( void )
{

}