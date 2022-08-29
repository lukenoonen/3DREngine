#ifndef TIMEMANAGER_H
#define TIMEMANAGER_H

#include "Global.h"

class CTimeManager
{
public:
	DECLARE_CLASS_NOBASE( CTimeManager )
	CTimeManager();

	bool			ShouldLoop( void );
	unsigned long	GetFrameCount( void );
	float			GetTime( void );
	float			GetFrameTime( void );
	float			GetTimeNoScale( void );
	float			GetFrameTimeNoScale( void );

	bool			MarkedForQuit( void );

	void			OnLoop( void );
	void			MarkForQuit( void );

private:
	unsigned long	m_ulFrameCount;

	float			m_flCurrentTime;
	float			m_flPreviousTime;

	float			m_flTime;
	float			m_flFrameTime;
	float			m_flTimeNoScale;
	float			m_flFrameTimeNoScale;

	bool			m_bMarkedForQuit;
};

#endif // TIMEMANAGER_H