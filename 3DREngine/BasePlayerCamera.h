#ifndef BASEPLAYERCAMERA_H
#define BASEPLAYERCAMERA_H

#include "Global.h"
#include "BaseUniformMSAACamera.h"

class CBasePlayerCamera : public CBaseUniformMSAACamera
{
public:
	DECLARE_CLASS( CBasePlayerCamera, CBaseUniformMSAACamera )

	CBasePlayerCamera();
};

#endif // BASEPLAYERCAMERA_H