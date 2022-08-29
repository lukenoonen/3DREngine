#include "BaseTexture.h"

CBaseTexture::CBaseTexture()
{
	m_bFiltered = true;
}

int CBaseTexture::Bind( void )
{
	return -1; // TODO: see if there's a more logical error value for this
}

void CBaseTexture::SetFiltered( bool bFiltered )
{
	m_bFiltered = bFiltered;
}

bool CBaseTexture::GetFiltered( void )
{
	return m_bFiltered;
}