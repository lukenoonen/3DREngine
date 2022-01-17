#ifndef BASEVARIABLEMSAACAMERA_H
#define BASEVARIABLEMSAACAMERA_H

#include "Global.h"
#include "BaseMSAACamera.h"

class CBaseVariableMSAACamera : public CBaseMSAACamera
{
public:
	DECLARE_CLASS( CBaseVariableMSAACamera, CBaseMSAACamera );

	CBaseVariableMSAACamera( unsigned int uiBaseSize, float flSizeRatio, unsigned int uiBaseMSAALevel, unsigned int uiRenderPriority );

	virtual void PreThink( void );

private:
	unsigned int m_uiBaseSize;
	float m_flSizeRatio;

	unsigned int m_uiBaseMSAALevel;
};

#endif // BASEVARIABLEMSAACAMERA_H