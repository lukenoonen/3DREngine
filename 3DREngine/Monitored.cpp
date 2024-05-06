#include "Monitored.h"
#include "TimeManager.h"

CMonitored::CMonitored()
{
	m_ulLastFrameModified = 0;
}

bool CMonitored::Modified( void ) const
{
	return m_ulLastFrameModified == pTimeManager->GetFrameCount();
}

void CMonitored::Modify( void )
{
	m_ulLastFrameModified = pTimeManager->GetFrameCount();
}