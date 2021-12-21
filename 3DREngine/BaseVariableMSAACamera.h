#ifndef BASEVARIABLEMSAACAMERA_H
#define BASEVARIABLEMSAACAMERA_H

#include "Global.h"
#include "BaseMSAACamera.h"

class CBaseVariableMSAACamera : public CBaseMSAACamera
{
public:
	DECLARE_CLASS( CBaseVariableMSAACamera, CBaseMSAACamera );

	CBaseVariableMSAACamera( unsigned int uiBaseSize, float flSizeRatio, unsigned int uiBaseMSAALevel, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PreThink( void );

private:
	unsigned int m_uiBaseSize;
	float m_flSizeRatio;

	unsigned int m_uiBaseMSAALevel;
};

#endif // BASEVARIABLEMSAACAMERA_H