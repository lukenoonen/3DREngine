#ifndef BASEWORLDCAMERA_H
#define BASEWORLDCAMERA_H

#include "Global.h"
#include "BaseVariableMSAACamera.h"

class CBaseWorldCamera : public CBaseVariableMSAACamera
{
public:
	DECLARE_CLASS( CBaseWorldCamera, CBaseVariableMSAACamera )

	CBaseWorldCamera();
};

#endif // BASEWORLDCAMERA_H