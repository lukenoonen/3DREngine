#ifndef BASEPLAYERCAMERA_H
#define BASEPLAYERCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"

class CBasePlayerCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( CBasePlayerCamera, CBaseWorldCamera )

	CBasePlayerCamera();

	virtual CBaseFramebuffer *GetFramebuffer( void );
	virtual const CBaseFramebuffer *GetFramebuffer( void ) const;
};

#endif // BASEPLAYERCAMERA_H