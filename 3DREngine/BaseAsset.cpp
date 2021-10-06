#include "BaseAsset.h"

CBaseAsset::CBaseAsset( const char *sPath )
{
	m_sPath = UTIL_stredit( sPath );
	m_uiActive = 0;
}

CBaseAsset::~CBaseAsset()
{
	delete[] m_sPath;
}

const char *CBaseAsset::GetPath( void ) const
{
	return m_sPath;
}

void CBaseAsset::Activate( void )
{
	m_uiActive++;
}

void CBaseAsset::Inactivate( void )
{
	m_uiActive--;
}

bool CBaseAsset::IsActive( void ) const
{
	return m_uiActive != 0;
}