#include "BaseCopySizeCamera.h"
#include "RenderManager.h"

CBaseCopySizeCamera::CBaseCopySizeCamera()
{
	m_pTargetCamera = NULL;

	m_flSizeQualityFactor = 1.0f;
}

bool CBaseCopySizeCamera::Init()
{
	if (!BaseClass::Init())
		return false;

	if (m_pTargetCamera)
	{
		SetSize( glm::ivec2( glm::vec2( m_pTargetCamera->GetSize() ) * m_flSizeQualityFactor * cf_r_vcsizefactor.GetValue() ) );
		m_vec2TargetCameraSize = m_pTargetCamera->GetSize();
		m_bUpdateSize = false;
	}

	return true;
}

void CBaseCopySizeCamera::PostThink( void )
{
	if (m_pTargetCamera && (m_bUpdateSize || m_vec2TargetCameraSize != m_pTargetCamera->GetSize() || cf_r_vcsizefactor.WasDispatched()))
	{
		SetSize( glm::ivec2( glm::vec2( m_pTargetCamera->GetSize() ) * m_flSizeQualityFactor * cf_r_vcsizefactor.GetValue() ) );
		m_vec2TargetCameraSize = m_pTargetCamera->GetSize();
		m_bUpdateSize = false;
	}

	return BaseClass::PostThink();
}

void CBaseCopySizeCamera::SetTargetCamera( CBaseSizeCamera *pTargetCamera )
{
	m_pTargetCamera = pTargetCamera;
	m_bUpdateSize = true;
}

void CBaseCopySizeCamera::SetSizeQualityFactor( float flSizeQualityFactor )
{
	m_flSizeQualityFactor = flSizeQualityFactor;
	m_bUpdateSize = true;
}