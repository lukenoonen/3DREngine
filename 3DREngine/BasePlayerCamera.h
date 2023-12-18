#ifndef BASEPLAYERCAMERA_H
#define BASEPLAYERCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"

class CBasePlayerCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( CBasePlayerCamera, CBaseWorldCamera )

	CBasePlayerCamera();
};

#endif // BASEPLAYERCAMERA_H