#ifndef BASECOLORCAMERA_H
#define BASECOLORCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"

class CBaseColorCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( CBaseColorCamera, CBaseWorldCamera )

	DECLARE_DATADESC()

	CBaseColorCamera();
};

#endif // BASECOLORCAMERA_H