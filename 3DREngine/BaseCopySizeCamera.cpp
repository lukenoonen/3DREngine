#include "BaseCopySizeCamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( CBaseCopySizeCamera )

	DEFINE_FIELD( LinkedDataField, CHandle<CBaseSizeCamera>, m_hTargetCamera, "targetcamera", 0 )
	DEFINE_FIELD( DataField, float, m_flSizeQualityFactor, "sizequalityfactor", 0 )

END_DATADESC()

CBaseCopySizeCamera::CBaseCopySizeCamera()
{
	m_flSizeQualityFactor = 1.0f;
}

bool CBaseCopySizeCamera::Init()
{
	if (!BaseClass::Init())
		return false;

	if (m_hTargetCamera)
	{
		SetSize( glm::ivec2( glm::vec2( m_hTargetCamera->GetSize() ) * m_flSizeQualityFactor * cf_r_vcsizefactor.GetValue() ) );
		m_vec2TargetCameraSize = m_hTargetCamera->GetSize();
		m_bUpdateSize = false;
	}

	return true;
}

void CBaseCopySizeCamera::PostThink( void )
{
	if (m_hTargetCamera && (m_bUpdateSize || m_vec2TargetCameraSize != m_hTargetCamera->GetSize() || cf_r_vcsizefactor.WasDispatched()))
	{
		SetSize( glm::ivec2( glm::vec2( m_hTargetCamera->GetSize() ) * m_flSizeQualityFactor * cf_r_vcsizefactor.GetValue() ) );
		m_vec2TargetCameraSize = m_hTargetCamera->GetSize();
		m_bUpdateSize = false;
	}

	m_hTargetCamera.Check();

	BaseClass::PostThink();
}

void CBaseCopySizeCamera::SetTargetCamera( CBaseSizeCamera *pTargetCamera )
{
	m_hTargetCamera = pTargetCamera;
	m_bUpdateSize = true;
}

void CBaseCopySizeCamera::SetSizeQualityFactor( float flSizeQualityFactor )
{
	m_flSizeQualityFactor = flSizeQualityFactor;
	m_bUpdateSize = true;
}

CBaseSizeCamera *CBaseCopySizeCamera::GetTargetCamera( void )
{
	return m_hTargetCamera;
}