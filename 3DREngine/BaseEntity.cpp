#include "BaseEntity.h"

CBaseEntity::CBaseEntity()
{
	m_bActive = true;

	m_uiEntityIndex = 0;
}

void CBaseEntity::Init( void )
{

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

void CBaseEntity::Exit( void )
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

bool CBaseEntity::IsDrawable( void ) const
{
	return false;
}

bool CBaseEntity::IsCamera( void ) const
{
	return false;
}

bool CBaseEntity::IsActive( void ) const
{
	return m_bActive;
}

void CBaseEntity::SetActive( bool bActive )
{
	m_bActive = bActive;
}

unsigned int CBaseEntity::GetEntityIndex( void ) const
{
	return m_uiEntityIndex;
}

void CBaseEntity::SetEntityIndex( unsigned int uiEntityIndex )
{
	m_uiEntityIndex = uiEntityIndex;
}