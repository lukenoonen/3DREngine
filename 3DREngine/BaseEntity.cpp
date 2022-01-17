#include "BaseEntity.h"

CBaseEntity::CBaseEntity()
{
	m_bActive = true;
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