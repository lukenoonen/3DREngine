#include "BaseSizeCamera.h"

CBaseSizeCamera::CBaseSizeCamera( const glm::vec2 &vecSize, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( uiRenderPriority, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_vecSize = vecSize;
}

void CBaseSizeCamera::SetSize( const glm::ivec2 vecSize )
{
	if (m_vecSize != vecSize)
	{
		DestroyTextureBuffers();
		DestroyMSAABuffers();

		m_vecSize = vecSize;

		CreateMSAABuffers();
		CreateTextureBuffers();
	}
}

const glm::ivec2 &CBaseSizeCamera::GetSize( void ) const
{
	return m_vecSize;
}