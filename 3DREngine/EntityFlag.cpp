#include "EntityFlag.h"
#include "EntityManager.h"

CEntityFlag::CEntityFlag( const char *sKey, int iFlag )
{
	m_sKey = sKey;
	m_iFlag = iFlag == -1 ? 0 : 1 << iFlag;

	CEntityManager::AddFlag( this );
}