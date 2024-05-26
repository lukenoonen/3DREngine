#ifndef BASECOLORCAMERA_H
#define BASECOLORCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"
#include "FramebufferColor.h"

class CBaseColorCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( CBaseColorCamera, CBaseWorldCamera )

	DECLARE_DATADESC()

	CBaseColorCamera();

	virtual CBaseFramebuffer *GetFramebuffer( void );
	virtual const CBaseFramebuffer *GetFramebuffer( void ) const;

private:
	CFramebufferColor m_fFramebufferColor;
};

#endif // BASECOLORCAMERA_H