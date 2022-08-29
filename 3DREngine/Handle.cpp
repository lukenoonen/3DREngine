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
	m_uData.sName = UTIL_StringEdit( sName );
}

void CBaseHandle::SetIndex( unsigned int uiIndex )
{
	m_ucActiveData = 2;
	m_uData.uiIndex = uiIndex;
}

CBaseEntity *CBaseHandle::Get( void )
{
	if (m_ucActiveData != 0)
	{
		CBaseEntity *pEntity = NULL;
		switch (m_ucActiveData)
		{
		case 1:
			pEntity = pEntityManager->GetEntityByName( m_uData.sName, NULL );
			delete[] m_uData.sName;
			break;
		case 2:
			pEntity = pEntityManager->GetEntityByIndex( m_uData.uiIndex, NULL );
			break;
		}

		if (Verify( pEntity ))
			SetEntity( pEntity );
		else
			SetEntity( NULL );
	}

	if (m_uData.pEntity && m_uData.pEntity->IsRemoved())
		SetEntity( NULL );

	return m_uData.pEntity;
}

bool CBaseHandle::Verify( CBaseEntity *pEntity )
{
	return pEntity != NULL;
}