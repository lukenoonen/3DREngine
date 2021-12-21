#ifndef BASEPLAYERCAMERA_H
#define BASEPLAYERCAMERA_H

#include "Global.h"
#include "BaseUniformMSAACamera.h"

class CBasePlayerCamera : public CBaseUniformMSAACamera
{
public:
	DECLARE_CLASS( CBasePlayerCamera, CBaseUniformMSAACamera );

	CBasePlayerCamera( unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
};

#endif // BASEPLAYERCAMERA_H