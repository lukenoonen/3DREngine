#include "BaseSizeCamera.h"

CBaseSizeCamera::CBaseSizeCamera()
{
	m_vecSize = g_vecOne * 1024.0f;
}

void CBaseSizeCamera::SetSize( const glm::ivec2 vecSize )
{
	if (m_vecSize != vecSize)
	{
		m_vecSize = vecSize;

		SetUpdateTextureBuffers( true );
		SetUpdateMSAABuffers( true );
	}
}

const glm::ivec2 &CBaseSizeCamera::GetSize( void ) const
{
	return m_vecSize;
}