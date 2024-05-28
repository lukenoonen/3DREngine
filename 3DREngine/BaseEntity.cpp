#include "BaseEntity.h"
#include "EntityManager.h"

DEFINE_DATADESC_NOBASE( CBaseEntity )

	DEFINE_FIELD( DataField, char *, m_sName, "name", FL_NONE )
	DEFINE_FIELD( FlagDataField, int, m_iFlags, "flags", FL_NONE )

END_DATADESC()

CBaseEntity::CBaseEntity()
{
	m_sName = NULL;
	m_sFileName = NULL;

	m_iFlags = FL_NONE;

	m_bRemoved = false;
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

void CBaseEntity::Remove( void )
{
	if (!m_bRemoved)
	{
		m_pLoadGroup->RemoveEntity( this );
		m_bRemoved = true;
	}
}

bool CBaseEntity::IsRemoved( void ) const
{
	return m_bRemoved;
}

void CBaseEntity::CleanUp( void )
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

int CBaseEntity::GetFlags( void ) const
{
	return m_iFlags;
}

CEntityLoadGroup *CBaseEntity::GetLoadGroup( void ) const
{
	return m_pLoadGroup;
}

void CBaseEntity::SetLoadGroup( CEntityLoadGroup *pLoadGroup )
{
	if (m_pLoadGroup) // TODO: refactor
		pLoadGroup->ChangeLoadGroup( this );
	m_pLoadGroup = pLoadGroup;
}

unsigned int CBaseEntity::GetLoadIndex( void ) const
{
	return m_pLoadGroup->GetEntityIndex( this );
}