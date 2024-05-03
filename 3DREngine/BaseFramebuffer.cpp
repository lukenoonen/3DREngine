#include "BaseFramebuffer.h"

CBaseFramebuffer::CBaseFramebuffer()
{

}

CBaseFramebuffer::~CBaseFramebuffer()
{
	DestroyTextureBuffers();
	DestroyMSAABuffers();
}

void CBaseFramebuffer::Init( void )
{
	CreateTextureBuffers();
	CreateMSAABuffers();
}

void CBaseFramebuffer::Think( void )
{

}

int CBaseFramebuffer::Bind( void ) const
{
	return -1;
}

GLuint CBaseFramebuffer::GetFramebuffer( void ) const
{
	return 0;
}

void CBaseFramebuffer::Blit( void )
{

}

const glm::ivec2 &CBaseFramebuffer::GetSize( void ) const
{
	return g_ivec2Zero;
}

unsigned char CBaseFramebuffer::GetMSAALevel( void ) const
{
	return 0;
}

void CBaseFramebuffer::SetSize( const glm::ivec2 &vec2Size )
{
	if (SetSizeInternal( vec2Size ))
	{
		DestroyTextureBuffers();
		CreateTextureBuffers();

		DestroyMSAABuffers();
		CreateMSAABuffers();
	}
}

void CBaseFramebuffer::SetMSAALevel( unsigned char ucMSAALevel )
{
	if (SetMSAALevelInternal( ucMSAALevel ))
	{
		DestroyMSAABuffers();
		CreateMSAABuffers();
	}
}

bool CBaseFramebuffer::SetSizeInternal( const glm::ivec2 &vec2Size )
{
	return false;
}

bool CBaseFramebuffer::SetMSAALevelInternal( unsigned char ucMSAALevel )
{
	return false;
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