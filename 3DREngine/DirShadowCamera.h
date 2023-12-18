#ifndef DIRSHADOWCAMERA_H
#define DIRSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CDirShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CDirShadowCamera, CBaseShadowCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CDirShadowCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void PerformRender( void );

	virtual void ActivateLight( void );

	void SetWidth( float flWidth );
	void SetNear( float flNear );
	void SetFar( float flFar );
	void SetBlurRadius( float flBlurRadius );

protected:
	virtual void PerformRender( void );

	virtual void UpdateView( void );
	virtual void UpdateProjection( void );
	virtual void UpdateTotal( void );

private:
	float m_flWidth;
	float m_flNear;
	float m_flFar;
	float m_flBlurRadius;

	bool m_bUpdateBlurScale;
};

#endif // DIRSHADOWCAMERA_H