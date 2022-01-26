#ifndef GLOBALMANAGER_H
#define GLOBALMANAGER_H

#include "Global.h"

class CGlobalValues
{
public:
	DECLARE_CLASS_NOBASE( CGlobalValues );
	CGlobalValues();

	bool			ShouldLoop( void );
	unsigned int	GetFrameCount( void );
	float			GetTime( void );
	float			GetFrameTime( void );
	float			GetTimeNoScale( void );
	float			GetFrameTimeNoScale( void );

	bool			MarkedForQuit( void );

	void			OnLoop( void );
	void			MarkForQuit( void );

private:
	unsigned int	m_uiFrameCount;

	float			m_flCurrentTime;
	float			m_flPreviousTime;

	float			m_flTime;
	float			m_flFrameTime;
	float			m_flTimeNoScale;
	float			m_flFrameTimeNoScale;

	bool			m_bMarkedForQuit;
};

#endif // GLOBALMANAGER_H