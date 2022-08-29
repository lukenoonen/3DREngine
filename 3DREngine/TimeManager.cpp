#include "TimeManager.h"
#include "CommandManager.h"

CConFloat cf_fps( 60.0f, "fps" );
CConFloat cf_timescale( 1.0f, "ts" );

bool CC_Quit( void )
{
	pTimeManager->MarkForQuit();
	return true;
}
CConCommand cc_quit( "quit", CC_Quit );

CTimeManager::CTimeManager()
{
	m_flCurrentTime = 0.0f;
	m_flPreviousTime = 0.0f;

	m_ulFrameCount = 1;

	m_flTime = 0.0f;
	m_flFrameTime = 0.0f;
	m_flTimeNoScale = 0.0f;
	m_flFrameTimeNoScale = 0.0f;

	m_bMarkedForQuit = false;
}

bool CTimeManager::ShouldLoop( void )
{
	m_flCurrentTime = (float)glfwGetTime();

	float flSecondsPerFrame = 1.0f / cf_fps.GetValue();
	if (flSecondsPerFrame < 0.0)
		return true;

	return m_flCurrentTime - m_flPreviousTime > flSecondsPerFrame;
}

unsigned long CTimeManager::GetFrameCount( void )
{
	return m_ulFrameCount;
}

float CTimeManager::GetTime( void )
{
	return m_flTime;
}

float CTimeManager::GetFrameTime( void )
{
	return m_flFrameTime;
}

float CTimeManager::GetTimeNoScale( void )
{
	return m_flTimeNoScale;
}

float CTimeManager::GetFrameTimeNoScale( void )
{
	return m_flFrameTimeNoScale;
}

bool CTimeManager::MarkedForQuit( void )
{
	return m_bMarkedForQuit;
}

void CTimeManager::OnLoop( void )
{
	m_ulFrameCount++;

	m_flFrameTimeNoScale = m_flCurrentTime - m_flPreviousTime;
	m_flFrameTime = m_flFrameTimeNoScale * cf_timescale.GetValue();

	m_flTime += m_flFrameTime;
	m_flTimeNoScale += m_flFrameTimeNoScale;

	m_flPreviousTime = m_flCurrentTime;
}

void CTimeManager::MarkForQuit( void )
{
	m_bMarkedForQuit = true;
}