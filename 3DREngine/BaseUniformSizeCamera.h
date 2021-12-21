#ifndef BASEUNIFORMSIZECAMERA_H
#define BASEUNIFORMSIZECAMERA_H

#include "Global.h"
#include "BaseSizeCamera.h"

class CBaseUniformSizeCamera : public CBaseSizeCamera
{
public:
	DECLARE_CLASS( CBaseUniformSizeCamera, CBaseSizeCamera );

	CBaseUniformSizeCamera( unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PreThink( void );
};

#endif // BASEUNIFORMSIZECAMERA_H