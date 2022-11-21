#include "BaseCopyMSAACamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( CBaseCopyMSAACamera )

	DEFINE_FIELD( LinkedDataField, CHandle<CBaseMSAACamera>, m_hTargetCamera, "targetcamera", 0 )
	DEFINE_FIELD( DataField, float, m_flSizeQualityFactor, "sizequalityfactor", 0 )
	DEFINE_FIELD( DataField, float, m_flMSAALevelQualityFactor, "msaalevelqualityfactor", 0 )

END_DATADESC()

CBaseCopyMSAACamera::CBaseCopyMSAACamera()
{
	m_flSizeQualityFactor = 1.0f;
	m_flMSAALevelQualityFactor = 1.0f;
}

bool CBaseCopyMSAACamera::Init()
{
	if (!BaseClass::Init())
		return false;

	if (m_hTargetCamera)
	{
		SetSize( glm::ivec2( glm::vec2( m_hTargetCamera->GetSize() ) * m_flSizeQualityFactor * cf_r_vcsizefactor.GetValue() ) );
		m_vec2TargetCameraSize = m_hTargetCamera->GetSize();
		m_bUpdateSize = false;

		SetMSAALevel( (unsigned char)((float)m_hTargetCamera->GetMSAALevel() * m_flMSAALevelQualityFactor * cf_r_vcmsaalevelfactor.GetValue()) );
		m_ucTargetCameraMSAALevel = m_hTargetCamera->GetMSAALevel();
		m_bUpdateSize = false;
	}

	return true;
}

void CBaseCopyMSAACamera::PostThink( void )
{
	if (m_hTargetCamera)
	{
		if (m_bUpdateSize || m_vec2TargetCameraSize != m_hTargetCamera->GetSize() || cf_r_vcsizefactor.WasDispatched())
		{
			SetSize( glm::ivec2( glm::vec2( m_hTargetCamera->GetSize() ) * m_flSizeQualityFactor * cf_r_vcsizefactor.GetValue() ) );
			m_vec2TargetCameraSize = m_hTargetCamera->GetSize();
			m_bUpdateSize = false;
		}

		if (m_bUpdateMSAALevel || m_ucTargetCameraMSAALevel != m_hTargetCamera->GetMSAALevel() || cf_r_vcmsaalevelfactor.WasDispatched())
		{
			SetMSAALevel( (unsigned char)((float)m_hTargetCamera->GetMSAALevel() * m_flMSAALevelQualityFactor * cf_r_vcmsaalevelfactor.GetValue()) );
			m_ucTargetCameraMSAALevel = m_hTargetCamera->GetMSAALevel();
			m_bUpdateSize = false;
		}
	}

	m_hTargetCamera.Check();

	BaseClass::PostThink();
}

void CBaseCopyMSAACamera::SetTargetCamera( CBaseMSAACamera *pTargetCamera )
{
	m_hTargetCamera = pTargetCamera;
}

void CBaseCopyMSAACamera::SetSizeQualityFactor( float flSizeQualityFactor )
{
	m_flSizeQualityFactor = flSizeQualityFactor;
	m_bUpdateSize = true;
}

void CBaseCopyMSAACamera::SetMSAALevelQualityFactor( float flMSAALevelQualityFactor )
{
	m_flMSAALevelQualityFactor = flMSAALevelQualityFactor;
	m_bUpdateMSAALevel = true;
}

CBaseMSAACamera *CBaseCopyMSAACamera::GetTargetCamera( void )
{
	return m_hTargetCamera;
}