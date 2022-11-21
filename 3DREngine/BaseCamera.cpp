#include "BaseCamera.h"
#include "RenderManager.h"

CBaseCamera::CBaseCamera()
{

}

CBaseCamera::~CBaseCamera()
{
	DestroyTextureBuffers();
	DestroyMSAABuffers();
}

bool CBaseCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateTextureBuffers = false;
	m_bUpdateMSAABuffers = false;

	CreateTextureBuffers();
	CreateMSAABuffers();

	return true;
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

int CBaseCamera::BindTexture( void )
{
	return -1; // TODO: see if there is a better number than this
}

void CBaseCamera::SetUpdateTextureBuffers( bool bUpdateTextureBuffers )
{
	m_bUpdateTextureBuffers = bUpdateTextureBuffers;
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

void CBaseCamera::CreateMSAABuffers( void )
{

}

void CBaseCamera::DestroyMSAABuffers( void )
{

}