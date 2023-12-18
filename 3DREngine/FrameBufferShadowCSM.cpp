#include "FramebufferShadowCSM.h"

CFramebufferShadowCSM::CFramebufferShadowCSM()
{

}

void CFramebufferShadowCSM::CreateTextureBuffers( void )
{
	int iSizeX = m_vec2Size.x;
	m_vec2Size.x *= 4;

	BaseClass::CreateTextureBuffers();

	m_vec2Size.x = iSizeX;
}