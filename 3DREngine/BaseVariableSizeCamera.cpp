#include "BaseVariableSizeCamera.h"
#include "RenderManager.h"

CBaseVariableSizeCamera::CBaseVariableSizeCamera( unsigned int uiBaseSize, float flSizeRatio, unsigned int uiRenderPriority ) : BaseClass( glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize, (float)m_uiBaseSize * m_flSizeRatio ) * cf_r_vcsizefactor.GetValue() ) ), uiRenderPriority )
{
	m_uiBaseSize = uiBaseSize;
	m_flSizeRatio = flSizeRatio;
}

void CBaseVariableSizeCamera::PreThink( void )
{
	if (cf_r_vcsizefactor.WasDispatched())
		SetSize( glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize * m_flSizeRatio, (float)m_uiBaseSize ) * cf_r_vcsizefactor.GetValue() ) ) );
}