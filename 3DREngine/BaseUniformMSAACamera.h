#ifndef BASEUNIFORMMSAACAMERA_H
#define BASEUNIFORMMSAACAMERA_H

#include "Global.h"
#include "BaseMSAACamera.h"

class CBaseUniformMSAACamera : public CBaseMSAACamera
{
public:
	DECLARE_CLASS( CBaseUniformMSAACamera, CBaseMSAACamera );

	CBaseUniformMSAACamera( unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PreThink( void );
};

#endif // BASEUNIFORMMSAACAMERA_H