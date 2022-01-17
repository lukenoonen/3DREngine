#include "BaseVariableMSAACamera.h"
#include "RenderManager.h"

CBaseVariableMSAACamera::CBaseVariableMSAACamera( unsigned int uiBaseSize, float flSizeRatio, unsigned int uiBaseMSAALevel, unsigned int uiRenderPriority ) : BaseClass( (unsigned int)glm::round( (float)m_uiBaseMSAALevel * cf_r_vcmsaalevelfactor.GetValue() ), glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize, (float)m_uiBaseSize * m_flSizeRatio ) * cf_r_vcsizefactor.GetValue() ) ), uiRenderPriority )
{
	m_uiBaseSize = uiBaseSize;
	m_flSizeRatio = flSizeRatio;

	m_uiBaseMSAALevel = uiBaseMSAALevel;
}

void CBaseVariableMSAACamera::PreThink( void )
{
	if (cf_r_vcsizefactor.WasDispatched())
		SetSize( glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize * m_flSizeRatio, (float)m_uiBaseSize ) * cf_r_vcsizefactor.GetValue() ) ) );

	if (cf_r_vcmsaalevelfactor.WasDispatched())
		SetMSAALevel( (unsigned int)glm::round( (float)m_uiBaseMSAALevel * cf_r_vcmsaalevelfactor.GetValue() ) );
}