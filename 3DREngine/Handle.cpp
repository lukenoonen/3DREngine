#include "Handle.h"
#include "BaseEntity.h"
#include "EntityManager.h"

CBaseHandle::CBaseHandle()
{
	m_ucActiveData = 0;
	m_uData.pEntity = NULL;
}

CBaseHandle::CBaseHandle( CBaseEntity *pEntity )
{
	m_ucActiveData = 0;
	m_uData.pEntity = pEntity;
}

CBaseHandle::~CBaseHandle()
{
	if (m_ucActiveData == 1)
		delete[] m_uData.sName;
}

void CBaseHandle::SetEntity( CBaseEntity *pEntity )
{
	if (m_ucActiveData == 0 && m_uData.pEntity)
		m_uData.pEntity->Unreference();

	if (pEntity)
		pEntity->Reference();

	m_ucActiveData = 0;
	m_uData.pEntity = pEntity;
}

void CBaseHandle::SetName( const char *sName )
{
	m_ucActiveData = 1;
	m_uData.sName = UTIL_strdup( sName );
}

void CBaseHandle::SetIndex( unsigned int uiIndex )
{
	m_ucActiveData = 2;
	m_uData.uiIndex = uiIndex;
}

bool CBaseHandle::Link( void )
{
	if (m_ucActiveData != 0)
	{
		CBaseEntity *pEntity = NULL;
		switch (m_ucActiveData)
		{
		case 1:
			pEntity = pEntityManager->GetEntityByName( m_uData.sName );
			delete[] m_uData.sName;
			break;
		case 2:
			pEntity = pEntityManager->GetEntityByIndex( m_uData.uiIndex );
			break;
		}

		if (!Verify( pEntity ))
		{
			SetEntity( NULL );
			return false;
		}

		SetEntity( pEntity );
	}

	return m_uData.pEntity != NULL;
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

bool CBaseHandle::Verify( CBaseEntity *pEntity )
{
	return pEntity != NULL;
}

bool UTIL_Write( CFile *pFile, CBaseHandle &hData )
{
	unsigned int uiIndex = pEntityManager->GetEntityIndex( hData.Get() );
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