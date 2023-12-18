#ifndef PERSPECTIVECOLORCAMERA_H
#define PERSPECTIVECOLORCAMERA_H

#include "Global.h"
#include "BaseColorCamera.h"

class CPerspectiveColorCamera : public CBaseColorCamera
{
public:
	DECLARE_CLASS( CPerspectiveColorCamera, CBaseColorCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CPerspectiveColorCamera();

	void SetFOV( float flFOV );
	void SetNear( float flNear );
	void SetFar( float flFar );

protected:
	virtual void UpdateProjection( void );

private:
	float m_flFOV;
	float m_flNear;
	float m_flFar;
};

#endif // PERSPECTIVECOLORCAMERA_H