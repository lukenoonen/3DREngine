#ifndef BASESHADOWCAMERA_H
#define BASESHADOWCAMERA_H

#include "Global.h"
#include "BaseCamera.h"

class CBaseShadowCamera : public CBaseCamera
{
public:
	DECLARE_CLASS( CBaseShadowCamera, CBaseCamera )

	DECLARE_DATADESC()

	CBaseShadowCamera();

	virtual void ActivateLight( void );

	void SetFadeNear( float flFadeNear );
	void SetFadeFar( float flFadeFar );
	void SetBlurScale( float flBlurScale );

private:
	float m_flFadeNear;
	float m_flFadeFar;
	float m_flBlurScale;
};

#endif // BASESHADOWCAMERA_H