#include "BaseCamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( CBaseCamera )

	DEFINE_FIELD( DataField, int, m_iPriority, "priority", FL_NONE )

END_DATADESC()

CBaseCamera::CBaseCamera()
{
	m_iPriority = 0;
}

bool CBaseCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	CBaseFramebuffer *pFramebuffer = GetFramebuffer();
	if (pFramebuffer)
		pFramebuffer->Init();

	UpdateView();
	UpdateProjection();
	UpdateTotal();

	return true;
}

void CBaseCamera::Think( void )
{
	CBaseFramebuffer *pFramebuffer = GetFramebuffer();
	if (pFramebuffer)
		pFramebuffer->Think();

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
		UpdateTotal();

	BaseClass::Think();
}

bool CBaseCamera::IsCamera( void ) const
{
	return true;
}

void CBaseCamera::Render( void )
{
	CBaseFramebuffer *pFramebuffer = GetFramebuffer();
	pRenderManager->SetFramebuffer( pFramebuffer );
	PerformRender();
	pRenderManager->ClearFramebuffer();
}

int CBaseCamera::Bind( void )
{
	CBaseFramebuffer *pFramebuffer = GetFramebuffer();
	return pFramebuffer != NULL ? pFramebuffer->Bind() : -1; // TODO: see if there is a better default number than this
}

int CBaseCamera::GetPriority( void ) const
{
	return m_iPriority;
}

CBaseFramebuffer *CBaseCamera::GetFramebuffer( void ) 
{
	return NULL;
}

const CBaseFramebuffer *CBaseCamera::GetFramebuffer( void ) const
{
	return NULL;
}

const glm::vec3 &CBaseCamera::GetCameraPosition( void ) const
{
	return GetPosition();
}

const glm::quat &CBaseCamera::GetCameraRotation( void ) const
{
	return GetRotation();
}