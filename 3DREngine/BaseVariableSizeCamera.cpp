#include "BaseVariableSizeCamera.h"
#include "RenderManager.h"

CBaseVariableSizeCamera::CBaseVariableSizeCamera( unsigned int uiBaseSize, float flSizeRatio, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize, (float)m_uiBaseSize * m_flSizeRatio ) * cf_r_vcsizefactor.GetValue() ) ), uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_uiBaseSize = uiBaseSize;
	m_flSizeRatio = flSizeRatio;
}

void CBaseVariableSizeCamera::PreThink( void )
{
	if (cf_r_vcsizefactor.WasDispatched())
		SetSize( glm::ivec2( glm::round( glm::vec2( (float)m_uiBaseSize * m_flSizeRatio, (float)m_uiBaseSize ) * cf_r_vcsizefactor.GetValue() ) ) );
}