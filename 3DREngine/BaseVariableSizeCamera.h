#ifndef BASEVARIABLESIZECAMERA_H
#define BASEVARIABLESIZECAMERA_H

#include "Global.h"
#include "BaseSizeCamera.h"

class CBaseVariableSizeCamera : public CBaseSizeCamera
{
public:
	DECLARE_CLASS( CBaseVariableSizeCamera, CBaseSizeCamera );

	CBaseVariableSizeCamera( unsigned int uiBaseSize, float flSizeRatio, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PreThink( void );

private:
	unsigned int m_uiBaseSize;
	float m_flSizeRatio;
};

#endif // BASEVARIABLESIZECAMERA_H