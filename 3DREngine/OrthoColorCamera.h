#ifndef ORTHOCOLORCAMERA_H
#define ORTHOCOLORCAMERA_H

#include "Global.h"
#include "BaseColorCamera.h"

class COrthoColorCamera : public CBaseColorCamera
{
public:
	DECLARE_CLASS( COrthoColorCamera, CBaseColorCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	COrthoColorCamera();

	void SetWidth( float flWidth );
	void SetNear( float flNear );
	void SetFar( float flFar );

protected:
	virtual void UpdateProjection( void );

private:
	float m_flWidth;
	float m_flNear;
	float m_flFar;
};

#endif // ORTHOCOLORCAMERA_H