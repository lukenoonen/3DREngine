#include "BaseReferenced.h"
#include "BaseEntity.h"

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

#include <iostream>

bool CBaseReferenced::IsReferenced( void ) const
{
	std::cout << m_uiReferences << "\n";
	return m_uiReferences != 0;
}