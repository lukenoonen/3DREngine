#include "BaseEntity.h"
#include "EntityManager.h"

DEFINE_DATADESC_NOBASE( CBaseEntity )

	DEFINE_FIELD( DataField, char *, m_sName, "name", FL_REQUIRED )
	// DEFINE_FIELD( DataField, int, m_iFlags, "flags" ) // TODO: how should flags work?

END_DATADESC()

CBaseEntity::CBaseEntity()
{
	// m_sName = NULL;
	m_sFileName = NULL;

	m_iFlags = 0;

	m_bRemoved = false;
	m_uiReferences = 0;
}

CBaseEntity::~CBaseEntity()
{
	if (m_sName)
		delete[] m_sName;

	if (m_sFileName)
		delete[] m_sFileName;
}

bool CBaseEntity::Init( void )
{
	return true;
}

void CBaseEntity::PreThink( void )
{

}

void CBaseEntity::Think( void )
{

}

void CBaseEntity::PostThink( void )
{

}

bool CBaseEntity::IsPlayer( void ) const
{
	return false;
}

bool CBaseEntity::IsLight( void ) const
{
	return false;
}

bool CBaseEntity::IsCamera( void ) const
{
	return false;
}

bool CBaseEntity::IsDrawable( void ) const
{
	return false;
}

const char *CBaseEntity::GetName( void ) const
{
	return m_sName;
}

const char *CBaseEntity::GetFileName( void ) const
{
	return m_sFileName;
}

const char *CBaseEntity::GetMapName( void ) const
{
	return NULL;
}

bool CBaseEntity::HasFlags( int iFlags ) const
{
	return m_iFlags & iFlags;
}

void CBaseEntity::AddFlags( int iFlags )
{
	m_iFlags |= iFlags;
}

void CBaseEntity::RemoveFlags( int iFlags )
{
	m_iFlags &= ~iFlags;
}

void CBaseEntity::Remove( void )
{
	m_bRemoved = true;
}

bool CBaseEntity::IsRemoved( void ) const
{
	return m_bRemoved;
}

void CBaseEntity::Reference( void )
{
	m_uiReferences++;
}

void CBaseEntity::Unreference( void )
{
	m_uiReferences--;
}

bool CBaseEntity::IsReferenced( void ) const
{
	return m_uiReferences != 0;
}