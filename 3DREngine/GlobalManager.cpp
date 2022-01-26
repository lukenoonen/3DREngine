#include "GlobalManager.h"
#include "CommandManager.h"

CConFloat cf_fps( 60.0f, "fps" );
CConFloat cf_timescale( 1.0f, "ts" );

bool CC_Quit( void )
{
	pGlobalValues->MarkForQuit();
	return true;
}
CConCommand cc_quit( "quit", CC_Quit );

CGlobalValues::CGlobalValues()
{
	m_flCurrentTime = 0.0f;
	m_flPreviousTime = 0.0f;

	m_uiFrameCount = 1;

	m_flTime = 0.0f;
	m_flFrameTime = 0.0f;
	m_flTimeNoScale = 0.0f;
	m_flFrameTimeNoScale = 0.0f;

	m_bMarkedForQuit = false;
}

bool CGlobalValues::ShouldLoop( void )
{
	m_flCurrentTime = (float)glfwGetTime();

	float flSecondsPerFrame = 1.0f / cf_fps.GetValue();
	if (flSecondsPerFrame < 0.0)
		return true;

	return m_flCurrentTime - m_flPreviousTime > flSecondsPerFrame;
}

unsigned int CGlobalValues::GetFrameCount( void )
{
	return m_uiFrameCount;
}

float CGlobalValues::GetTime( void )
{
	return m_flTime;
}

float CGlobalValues::GetFrameTime( void )
{
	return m_flFrameTime;
}

float CGlobalValues::GetTimeNoScale( void )
{
	return m_flTimeNoScale;
}

float CGlobalValues::GetFrameTimeNoScale( void )
{
	return m_flFrameTimeNoScale;
}

bool CGlobalValues::MarkedForQuit( void )
{
	return m_bMarkedForQuit;
}

void CGlobalValues::OnLoop( void )
{
	m_uiFrameCount++;

	m_flFrameTimeNoScale = m_flCurrentTime - m_flPreviousTime;
	m_flFrameTime = m_flFrameTimeNoScale * cf_timescale.GetValue();

	m_flTime += m_flFrameTime;
	m_flTimeNoScale += m_flFrameTimeNoScale;

	m_flPreviousTime = m_flCurrentTime;
}

void CGlobalValues::MarkForQuit( void )
{
	m_bMarkedForQuit = true;
}