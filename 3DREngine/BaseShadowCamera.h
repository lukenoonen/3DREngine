#ifndef BASESHADOWCAMERA_H
#define BASESHADOWCAMERA_H

#include "Global.h"
#include "BaseVariableSizeCamera.h"

class CBaseShadowCamera : public CBaseVariableSizeCamera
{
public:
	DECLARE_CLASS( CBaseShadowCamera, CBaseVariableSizeCamera );

	CBaseShadowCamera( float flFadeNear, float flFadeFar, float flBlurScale, unsigned int uiBaseSize, float flSizeRatio, unsigned int uiRenderPriority );

	virtual void ActivateLight( void );

	void SetBlurScale( float flBlurScale );

private:
	float m_flFadeNear;
	float m_flFadeFar;

	float m_flBlurScale;
};

#endif // BASESHADOWCAMERA_H