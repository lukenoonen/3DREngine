#include "BaseCamera.h"
#include "RenderManager.h"

CBaseCamera::CBaseCamera()
{
	m_uiRenderPriority = 0;
}

void CBaseCamera::Init( void )
{
	m_bUpdateTextureBuffers = false;
	m_bUpdateMSAABuffers = false;

	CreateTextureBuffers();
	CreateMSAABuffers();

	BaseClass::Init();
}

void CBaseCamera::Exit( void )
{
	DestroyTextureBuffers();
	DestroyMSAABuffers();

	BaseClass::Exit();
}

void CBaseCamera::PostThink( void )
{
	if (m_bUpdateTextureBuffers)
	{
		DestroyTextureBuffers();
		CreateTextureBuffers();

		m_bUpdateTextureBuffers = false;
	}

	if (m_bUpdateMSAABuffers)
	{
		DestroyMSAABuffers();
		CreateMSAABuffers();

		m_bUpdateMSAABuffers = false;
	}

	BaseClass::PostThink();
}

bool CBaseCamera::IsCamera( void ) const
{
	return true;
}

void CBaseCamera::Render( void )
{

}

void CBaseCamera::SetRenderPriority( unsigned int uiRenderPriority )
{
	m_uiRenderPriority = uiRenderPriority; // TODO: potentially figure out a concise way to be able to change render priority on the fly
}

unsigned int CBaseCamera::GetRenderPriority( void ) const
{
	return m_uiRenderPriority;
}

void CBaseCamera::SetUpdateTextureBuffers( bool bUpdateTextureBuffers )
{
	m_bUpdateTextureBuffers = bUpdateTextureBuffers;
}

bool CBaseCamera::GetUpdateTextureBuffers( void ) const
{

}

void CBaseCamera::CreateTextureBuffers( void )
{

}

void CBaseCamera::DestroyTextureBuffers( void )
{

}

void CBaseCamera::SetUpdateMSAABuffers( bool bUpdateMSAABuffers )
{
	m_bUpdateMSAABuffers = bUpdateMSAABuffers;
}

bool CBaseCamera::GetUpdateMSAABuffers( void ) const
{

}

void CBaseCamera::CreateMSAABuffers( void )
{

}

void CBaseCamera::DestroyMSAABuffers( void )
{

}