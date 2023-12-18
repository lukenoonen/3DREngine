#include "BaseResource.h"

CBaseResource::CBaseResource( const char *sName )
{
	m_sName = UTIL_strdup( sName );
}

CBaseResource::~CBaseResource()
{
	delete[] m_sName;
}

const char *CBaseResource::GetName( void ) const
{
	return m_sName;
}