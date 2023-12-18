#ifndef SPOTSHADOWCAMERA_H
#define SPOTSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CSpotShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CSpotShadowCamera, CBaseShadowCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CSpotShadowCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void ActivateLight( void );

	void SetNear( float flNear );
	void SetFar( float flFar );
	void SetOuterCutoff( float flOuterCutoff );
	void SetBlurRadius( float flBlurRadius );

protected:
	virtual void PerformRender( void );

	virtual void UpdateView( void );
	virtual void UpdateProjection( void );
	virtual void UpdateTotal( void );

private:
	float m_flNear;
	float m_flFar;
	float m_flOuterCutoff;
	float m_flBlurRadius;

	bool m_bUpdateBlurScale;
};

#endif // SPOTSHADOWCAMERA_H