#ifndef ORTHOCOLORCAMERA_H
#define ORTHOCOLORCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"

class COrthoColorCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( COrthoColorCamera, CBaseWorldCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	COrthoColorCamera();

	void SetWidth( float flWidth );
	void SetNear( float flNear );
	void SetFar( float flFar );

protected:
	virtual bool ShouldUpdateProjection( void ) const;

	virtual glm::mat4 CalculateProjection( void ) const;

private:
	CMonitoredValue<float> m_flWidth;
	CMonitoredValue<float> m_flNear;
	CMonitoredValue<float> m_flFar;
};

#endif // ORTHOCOLORCAMERA_H