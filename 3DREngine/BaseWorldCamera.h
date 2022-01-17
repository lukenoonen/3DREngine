#ifndef BASEWORLDCAMERA_H
#define BASEWORLDCAMERA_H

#include "Global.h"
#include "BaseVariableMSAACamera.h"

class CBaseWorldCamera : public CBaseVariableMSAACamera
{
public:
	DECLARE_CLASS( CBaseWorldCamera, CBaseVariableMSAACamera );

	CBaseWorldCamera( unsigned int uiBaseSize, float flSizeRatio, unsigned int uiBaseMSAALevel, unsigned int uiRenderPriority );
};

#endif // BASEWORLDCAMERA_H