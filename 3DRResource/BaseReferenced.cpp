#include "BaseReferenced.h"

CBaseReferenced::CBaseReferenced()
{
	m_uiReferences = 0;
}

void CBaseReferenced::Reference( void )
{
	m_uiReferences++;
}

void CBaseReferenced::Unreference( void )
{
	m_uiReferences--;
}

bool CBaseReferenced::IsReferenced( void ) const
{
	return m_uiReferences != 0;
}