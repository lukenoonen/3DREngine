#ifndef POINTSHADOWCAMERA_H
#define POINTSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CPointShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CPointShadowCamera, CBaseShadowCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CPointShadowCamera();

	virtual void ActivateLight( void );

	void SetNear( float flNear );
	void SetFar( float flFar );
	void SetBlurRadius( float flBlurRadius );

protected:
	virtual void PerformRender( void );

	virtual void UpdateView( void );
	virtual void UpdateProjection( void );
	virtual void UpdateTotal( void );

	virtual bool ShouldUpdateView( void );

private:
	float m_flNear;
	float m_flFar;
};

#endif // POINTSHADOWCAMERA_H