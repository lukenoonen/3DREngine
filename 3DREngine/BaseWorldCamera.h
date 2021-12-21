#ifndef BASEWORLDCAMERA_H
#define BASEWORLDCAMERA_H

#include "Global.h"
#include "BaseVariableMSAACamera.h"

class CBaseWorldCamera : public CBaseVariableMSAACamera
{
public:
	DECLARE_CLASS( CBaseWorldCamera, CBaseVariableMSAACamera );

	CBaseWorldCamera( unsigned int uiBaseSize, float flSizeRatio, unsigned int uiBaseMSAALevel, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
};

#endif // BASEWORLDCAMERA_H