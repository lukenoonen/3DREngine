#include "BaseCopyCamera.h"
#include "FramebufferColor.h"

DEFINE_DATADESC( CBaseCopyCamera )

	DEFINE_FIELD( LinkedDataField, CHandle<CBaseWorldCamera>, m_hTargetCamera, "targetcamera", FL_REQUIRED )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flSizeQualityFactor, "sizequalityfactor", FL_NONE )
	DEFINE_FIELD( DataField, CMonitoredValue<float>, m_flMSAALevelQualityFactor, "msaalevelqualityfactor", FL_NONE )

END_DATADESC()

CBaseCopyCamera::CBaseCopyCamera()
{
	InitFramebuffer( new CFramebufferColor() );

	m_flSizeQualityFactor = 1.0f;
	m_flMSAALevelQualityFactor = 1.0f;
}

bool CBaseCopyCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	CBaseFramebuffer *pFramebuffer = GetFramebuffer();
	CBaseFramebuffer *pTargetFramebuffer = m_hTargetCamera->GetFramebuffer();

	const glm::ivec2 &vec2TargetSize = pTargetFramebuffer->GetSize();
	pFramebuffer->SetSize( glm::ivec2( glm::vec2( vec2TargetSize ) * m_flSizeQualityFactor.Get() ) );
	m_vec2TargetSize = vec2TargetSize;

	unsigned char ucTargetMSAALevel = pTargetFramebuffer->GetMSAALevel();
	pFramebuffer->SetMSAALevel( (unsigned char)((float)ucTargetMSAALevel * m_flMSAALevelQualityFactor.Get()) );
	m_ucTargetMSAALevel = ucTargetMSAALevel;

	return true;
}

void CBaseCopyCamera::Think( void )
{
	CBaseFramebuffer *pFramebuffer = GetFramebuffer();
	CBaseFramebuffer *pTargetFramebuffer = m_hTargetCamera->GetFramebuffer();

	const glm::ivec2 &vec2TargetSize = pTargetFramebuffer->GetSize();
	if (m_flSizeQualityFactor.Modified() || m_vec2TargetSize != vec2TargetSize)
	{
		pFramebuffer->SetSize( glm::ivec2( glm::vec2( vec2TargetSize ) * m_flSizeQualityFactor.Get() ) );
		m_vec2TargetSize = vec2TargetSize;
	}

	unsigned char ucTargetMSAALevel = pTargetFramebuffer->GetMSAALevel();
	if (m_flMSAALevelQualityFactor.Modified() || m_ucTargetMSAALevel != ucTargetMSAALevel)
	{
		pFramebuffer->SetMSAALevel( (unsigned char)((float)ucTargetMSAALevel * m_flMSAALevelQualityFactor.Get()) );
		m_ucTargetMSAALevel = ucTargetMSAALevel;
	}

	m_hTargetCamera.Check();

	BaseClass::Think();
}

CBaseWorldCamera *CBaseCopyCamera::GetTargetCamera( void ) const
{
	return m_hTargetCamera;
}