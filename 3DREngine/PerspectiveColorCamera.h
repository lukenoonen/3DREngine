#ifndef PERSPECTIVEWORLDCAMERA_H
#define PERSPECTIVEWORLDCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"

class CPerspectiveColorCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( CPerspectiveColorCamera, CBaseWorldCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CPerspectiveColorCamera();

	void SetFOV( float flFOV );
	void SetNear( float flNear );
	void SetFar( float flFar );

protected:
	virtual bool ShouldUpdateProjection( void ) const;

	virtual glm::mat4 CalculateProjection( void ) const;

private:
	CMonitoredValue<float> m_flFOV;
	CMonitoredValue<float> m_flNear;
	CMonitoredValue<float> m_flFar;
};

#endif // PERSPECTIVEWORLDCAMERA_H