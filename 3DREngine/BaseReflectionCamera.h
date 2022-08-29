#ifndef BASEREFLECTIONCAMERA_H
#define BASEREFLECTIONCAMERA_H

#include "Global.h"
#include "BaseCopyMSAACamera.h"

class CBaseReflectionCamera : public CBaseCopyMSAACamera
{
public:
	DECLARE_CLASS( CBaseReflectionCamera, CBaseCopyMSAACamera )

	CBaseReflectionCamera();
};

#endif // BASEREFLECTIONCAMERA_H