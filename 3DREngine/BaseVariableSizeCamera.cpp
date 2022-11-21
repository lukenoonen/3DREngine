#include "BaseVariableSizeCamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( CBaseVariableSizeCamera )

	DEFINE_FIELD( DataField, unsigned int, m_uiBaseSize, "basesize", 0 )
	DEFINE_FIELD( DataField, float, m_flSizeRatio, "sizeratio", 0 )

END_DATADESC()

CBaseVariableSizeCamera::CBaseVariableSizeCamera()
{
	m_uiBaseSize = 1024;  // TODO: maybe just make this a vec2?
	m_flSizeRatio = 1.0f;
}

bool CBaseVariableSizeCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_bUpdateSize = false;

	SetSize( glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize * m_flSizeRatio, (float)m_uiBaseSize ) * cf_r_vcsizefactor.GetValue() ) ) );

	return true;
}

void CBaseVariableSizeCamera::PostThink( void )
{
	if (m_bUpdateSize || cf_r_vcsizefactor.WasDispatched())
	{
		SetSize( glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize * m_flSizeRatio, (float)m_uiBaseSize ) * cf_r_vcsizefactor.GetValue() ) ) );
		m_bUpdateSize = false;
	}

	BaseClass::PostThink();
}

void CBaseVariableSizeCamera::SetBaseSize( unsigned int uiBaseSize )
{
	m_uiBaseSize = uiBaseSize;
	m_bUpdateSize = true;
}

void CBaseVariableSizeCamera::SetSizeRatio( float flSizeRatio )
{
	m_flSizeRatio = flSizeRatio;
	m_bUpdateSize = true;
}

unsigned int CBaseVariableSizeCamera::GetBaseSize( void ) const
{
	return m_uiBaseSize;
}

float CBaseVariableSizeCamera::GetSizeRatio( void ) const
{
	return m_flSizeRatio;
}