#include "Monitored.h"
#include "TimeManager.h"

CMonitored::CMonitored()
{
	m_uiLastFrameModified = 0;
}

bool CMonitored::Modified( void ) const
{
	return m_uiLastFrameModified == pTimeManager->GetFrameCount();
}

void CMonitored::Modify( void )
{
	m_uiLastFrameModified = pTimeManager->GetFrameCount();
}