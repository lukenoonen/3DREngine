#include "Handle.h"
#include "BaseEntity.h"
#include "EntityManager.h"

CBaseHandle::CBaseHandle()
{
	SetEntityInternal( NULL );
}

CBaseHandle::CBaseHandle( CBaseEntity *pEntity )
{
	SetEntityInternal( pEntity );
}

CBaseHandle::CBaseHandle( CBaseHandle &hOther )
{
	CopyOther( hOther );
}

CBaseHandle::~CBaseHandle()
{
	if (pEntityManager->IsClearing())
	{
		if (m_ucActiveData == 1 && m_uData.sName)
			delete[] m_uData.sName;
	}
	else
	{
		OnChangeData();
	}
}

void CBaseHandle::SetEntity( CBaseEntity *pEntity )
{
	OnChangeData();
	SetEntityInternal( pEntity );
}

void CBaseHandle::SetName( const char *sName )
{
	OnChangeData();
	SetNameInternal( sName );
}

void CBaseHandle::SetIndex( unsigned int uiIndex )
{
	OnChangeData();
	SetIndexInternal( uiIndex );
}

CBaseEntity *CBaseHandle::FindEntity( void ) const
{
	CBaseEntity *pEntity = NULL;
	switch (m_ucActiveData)
	{
	case 1:
		pEntity = pEntityManager->GetEntityByName( m_uData.sName );
		break;
	case 2:
		pEntity = pEntityManager->GetEntityByIndex( m_uData.uiIndex );
		break;
	}
	return pEntity;
}

void CBaseHandle::OnChangeData( void )
{
	switch (m_ucActiveData)
	{
	case 0:
		if (m_uData.pEntity)
			m_uData.pEntity->Unreference();
		break;
	case 1:
		delete[] m_uData.sName;
		break;
	}
}

CBaseEntity *CBaseHandle::Get( void ) const
{
	return m_uData.pEntity;
}

bool CBaseHandle::Check( void )
{
	if (m_uData.pEntity && m_uData.pEntity->IsRemoved())
		SetEntity( NULL );

	return m_uData.pEntity != NULL;
}

bool CBaseHandle::Link( void )
{
	if (m_ucActiveData != 0)
	{
		CBaseEntity *pEntity = FindEntity();
		if (!Verify( pEntity ))
			pEntity = NULL;

		SetEntity( pEntity );
	}

	return m_uData.pEntity != NULL;
}

bool CBaseHandle::Verify( CBaseEntity *pEntity ) const
{
	return pEntity != NULL;
}

void CBaseHandle::CopyOther( CBaseHandle &hOther )
{
	switch (hOther.m_ucActiveData)
	{
	case 0:
		SetEntityInternal( hOther.m_uData.pEntity );
		break;
	case 1:
		SetNameInternal( hOther.m_uData.sName );
		break;
	case 2:
		SetIndexInternal( hOther.m_uData.uiIndex );
		break;
	}
}

void CBaseHandle::SetEntityInternal( CBaseEntity *pEntity )
{
	if (pEntity)
		pEntity->Reference();

	m_ucActiveData = 0;
	m_uData.pEntity = pEntity;
}

void CBaseHandle::SetNameInternal( const char *sName )
{
	m_ucActiveData = 1;
	m_uData.sName = UTIL_strdup( sName );
}

void CBaseHandle::SetIndexInternal( unsigned int uiIndex )
{
	m_ucActiveData = 2;
	m_uData.uiIndex = uiIndex;
}

bool UTIL_Write( CFile *pFile, CBaseHandle &hData )
{
	CBaseEntity *pEntity = hData.Get();
	unsigned int uiIndex = pEntity->GetLoadIndex();
	return pFile->Write( uiIndex );
}

bool UTIL_Read( CFile *pFile, CBaseHandle &hData )
{
	unsigned int uiIndex;
	if (!pFile->Read( uiIndex ))
		return false;

	hData.SetIndex( uiIndex );
	return true;
}

bool UTIL_GetValue( const CTextItem *pTextItem, CBaseHandle &hValue )
{
	const char *sName;
	if (!pTextItem->GetValue( sName ))
		return false;

	if (UTIL_streq( sName, "NULL" )) // TODO: remove this test
		hValue.SetEntity( NULL );
	else
		hValue.SetName( sName );

	return true;
}