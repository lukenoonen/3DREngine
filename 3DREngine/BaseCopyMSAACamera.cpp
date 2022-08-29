#include "BaseCopyMSAACamera.h"
#include "RenderManager.h"

CBaseCopyMSAACamera::CBaseCopyMSAACamera()
{
	m_pTargetCamera = NULL;

	m_flSizeQualityFactor = 1.0f;
	m_flMSAALevelQualityFactor = 1.0f;
}

bool CBaseCopyMSAACamera::Init()
{
	if (!BaseClass::Init())
		return false;

	if (m_pTargetCamera)
	{
		SetSize( glm::ivec2( glm::vec2( m_pTargetCamera->GetSize() ) * m_flSizeQualityFactor * cf_r_vcsizefactor.GetValue() ) );
		m_vec2TargetCameraSize = m_pTargetCamera->GetSize();
		m_bUpdateSize = false;

		SetMSAALevel( (unsigned char)((float)m_pTargetCamera->GetMSAALevel() * m_flMSAALevelQualityFactor * cf_r_vcmsaalevelfactor.GetValue()) );
		m_ucTargetCameraMSAALevel = m_pTargetCamera->GetMSAALevel();
		m_bUpdateSize = false;
	}

	return true;
}

void CBaseCopyMSAACamera::PostThink( void )
{
	if (m_pTargetCamera)
	{
		if (m_bUpdateSize || m_vec2TargetCameraSize != m_pTargetCamera->GetSize() || cf_r_vcsizefactor.WasDispatched())
		{
			SetSize( glm::ivec2( glm::vec2( m_pTargetCamera->GetSize() ) * m_flSizeQualityFactor * cf_r_vcsizefactor.GetValue() ) );
			m_vec2TargetCameraSize = m_pTargetCamera->GetSize();
			m_bUpdateSize = false;
		}

		if (m_bUpdateMSAALevel || m_ucTargetCameraMSAALevel != m_pTargetCamera->GetMSAALevel() || cf_r_vcmsaalevelfactor.WasDispatched())
		{
			SetMSAALevel( (unsigned char)((float)m_pTargetCamera->GetMSAALevel() * m_flMSAALevelQualityFactor * cf_r_vcmsaalevelfactor.GetValue()) );
			m_ucTargetCameraMSAALevel = m_pTargetCamera->GetMSAALevel();
			m_bUpdateSize = false;
		}
	}

	BaseClass::PostThink();
}

void CBaseCopyMSAACamera::SetTargetCamera( CBaseMSAACamera *pTargetCamera )
{
	m_pTargetCamera = pTargetCamera;
	m_bUpdateSize = true;
	m_bUpdateMSAALevel = true;
}

void CBaseCopyMSAACamera::SetSizeQualityFactor( float flSizeQualityFactor )
{
	m_flSizeQualityFactor = flSizeQualityFactor;
	m_bUpdateSize = true;
}

void CBaseCopyMSAACamera::SetMSAALevelQualityFactor( float flMSAAQualityFactor )
{
	m_flMSAALevelQualityFactor = flMSAAQualityFactor;
	m_bUpdateMSAALevel = true;
}

CBaseMSAACamera *CBaseCopyMSAACamera::GetTargetCamera( void )
{
	return m_pTargetCamera;
}