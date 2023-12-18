#include "BaseCamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( CBaseCamera )

	DEFINE_FIELD( DataField, int, m_iPriority, "priority", 0 )

END_DATADESC()

CBaseCamera::CBaseCamera()
{
	m_iPriority = 0;

	m_pFramebuffer = NULL;
}

CBaseCamera::~CBaseCamera()
{
	if (m_pFramebuffer)
		delete m_pFramebuffer;
}

bool CBaseCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateProjection = false;

	UpdateView();
	UpdateProjection();
	UpdateTotal();

	return true;
}

void CBaseCamera::PostThink( void )
{
	bool bUpdateTotal = false;

	if (ShouldUpdateView())
	{
		UpdateView();
		bUpdateTotal = true;
	}

	if (ShouldUpdateProjection())
	{
		UpdateProjection();
		bUpdateTotal = true;
	}

	if (bUpdateTotal)
	{
		UpdateTotal();
	}

	m_bUpdateProjection = false;

	BaseClass::PostThink();
}

bool CBaseCamera::IsCamera( void ) const
{
	return true;
}

void CBaseCamera::Render( void )
{
	pRenderManager->SetFramebuffer( m_pFramebuffer );
	PerformRender();
	pRenderManager->ClearFramebuffer();
}

int CBaseCamera::Bind( void )
{
	return m_pFramebuffer != NULL ? m_pFramebuffer->Bind() : -1; // TODO: see if there is a better default number than this
}

int CBaseCamera::GetPriority( void ) const
{
	return m_iPriority;
}

void CBaseCamera::PerformRender( void )
{

}

void CBaseCamera::UpdateView( void )
{

}

void CBaseCamera::UpdateProjection( void )
{

}

void CBaseCamera::UpdateTotal( void )
{

}

bool CBaseCamera::ShouldUpdateView( void )
{
	return PositionUpdated() || RotationUpdated();
}

bool CBaseCamera::ShouldUpdateProjection( void )
{
	return m_bUpdateProjection;
}

void CBaseCamera::MarkUpdateProjection( void )
{
	m_bUpdateProjection = true;
}