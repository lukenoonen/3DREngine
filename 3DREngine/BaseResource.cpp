#include "BaseResource.h"

CBaseResource::CBaseResource( const char *sName )
{
	m_sName = UTIL_strdup( sName );
	m_uiReferences = 0;
}

CBaseResource::~CBaseResource()
{
	delete[] m_sName;
}

const char *CBaseResource::GetName( void ) const
{
	return m_sName;
}

void CBaseResource::Reference( void )
{
	m_uiReferences++;
}

void CBaseResource::Unreference( void )
{
	m_uiReferences--;
}

bool CBaseResource::IsReferenced( void ) const
{
	return m_uiReferences != 0;
}