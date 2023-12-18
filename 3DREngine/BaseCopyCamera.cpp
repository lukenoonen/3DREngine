#include "BaseCopyCamera.h"
#include "FramebufferColor.h"

DEFINE_DATADESC( CBaseCopyCamera )

	DEFINE_FIELD( LinkedDataField, CBaseWorldCamera, m_hTarget, "target", FL_REQUIRED )

END_DATADESC()

CBaseCopyCamera::CBaseCopyCamera()
{
	m_pFramebuffer = new CFramebufferColor();
}

bool CBaseCopyCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	// TODO: copy the target camera framebuffer size!

	return true;
}

CBaseWorldCamera *CBaseCopyCamera::GetTarget( void ) const
{
	return m_hTarget;
}

void CBaseCopyCamera::UpdateView( void )
{

}

void CBaseCopyCamera::UpdateTotal( void )
{

}