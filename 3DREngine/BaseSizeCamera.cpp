#include "BaseSizeCamera.h"

DEFINE_DATADESC( CBaseSizeCamera )

	DEFINE_FIELD( DataField, glm::vec2, m_vec2Size, "size", 0 )

END_DATADESC()

CBaseSizeCamera::CBaseSizeCamera()
{
	m_vec2Size = glm::ivec2( g_vec2One ) * 1024;
}

void CBaseSizeCamera::SetSize( const glm::ivec2 vec2Size )
{
	if (m_vec2Size != vec2Size)
	{
		m_vec2Size = vec2Size;

		SetUpdateTextureBuffers( true );
		SetUpdateMSAABuffers( true );
	}
}

const glm::ivec2 &CBaseSizeCamera::GetSize( void ) const
{
	return m_vec2Size;
}