#include "BaseCamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( CBaseCamera )

	DEFINE_FIELD( DataField, int, m_iPriority, "priority", FL_NONE )

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

	if (m_pFramebuffer)
		m_pFramebuffer->Init();

	UpdateView();
	UpdateProjection();
	UpdateTotal();

	return true;
}

void CBaseCamera::Think( void )
{
	if (m_pFramebuffer)
		m_pFramebuffer->Think();

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

void CBaseCamera::InitFramebuffer( CBaseFramebuffer *pFramebuffer )
{
	m_pFramebuffer = pFramebuffer;
}

CBaseFramebuffer *CBaseCamera::GetFramebuffer( void ) const
{
	return m_pFramebuffer;
}

const glm::vec3 &CBaseCamera::GetCameraPosition( void ) const
{
	return GetPosition();
}

const glm::quat &CBaseCamera::GetCameraRotation( void ) const
{
	return GetRotation();
}