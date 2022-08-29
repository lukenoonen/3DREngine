#include "BaseVariableMSAACamera.h"
#include "RenderManager.h"

CBaseVariableMSAACamera::CBaseVariableMSAACamera()
{
	m_uiBaseSize = 1024;
	m_flSizeRatio = 1.0f;

	m_ucBaseMSAALevel = 0;
}

bool CBaseVariableMSAACamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateSize = false;
	m_bUpdateMSAA = false;

	SetSize( glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize * m_flSizeRatio, (float)m_uiBaseSize ) * cf_r_vcsizefactor.GetValue() ) ) );

	SetMSAALevel( (unsigned char)glm::round( (float)m_ucBaseMSAALevel * cf_r_vcmsaalevelfactor.GetValue() ) );

	return true;
}

void CBaseVariableMSAACamera::PostThink( void )
{
	if (m_bUpdateSize || cf_r_vcsizefactor.WasDispatched())
	{
		SetSize( glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize * m_flSizeRatio, (float)m_uiBaseSize ) * cf_r_vcsizefactor.GetValue() ) ) );
		m_bUpdateSize = false;
	}

	if (m_bUpdateMSAA || cf_r_vcmsaalevelfactor.WasDispatched())
	{
		SetMSAALevel( (unsigned char)glm::round( (float)m_ucBaseMSAALevel * cf_r_vcmsaalevelfactor.GetValue() ) );
		m_bUpdateMSAA = false;
	}

	BaseClass::PostThink();
}

void CBaseVariableMSAACamera::SetBaseSize( unsigned int uiBaseSize )
{
	m_uiBaseSize = uiBaseSize;
	m_bUpdateSize = true;
}

void CBaseVariableMSAACamera::SetSizeRatio( float flSizeRatio )
{
	m_flSizeRatio = flSizeRatio;
	m_bUpdateSize = true;
}

void CBaseVariableMSAACamera::SetBaseMSAALevel( unsigned char ucBaseMSAALevel )
{
	m_ucBaseMSAALevel = ucBaseMSAALevel;
	m_bUpdateMSAA = true;
}

float CBaseVariableMSAACamera::GetSizeRatio( void ) const
{
	return m_flSizeRatio;
}