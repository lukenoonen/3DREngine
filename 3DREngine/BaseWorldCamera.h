#ifndef BASEWORLDCAMERA_H
#define BASEWORLDCAMERA_H

#include "Global.h"
#include "BaseCamera.h"

class CBaseWorldCamera : public CBaseCamera
{
public:
	DECLARE_CLASS( CBaseWorldCamera, CBaseCamera )

	CBaseWorldCamera();

protected:
	virtual void PerformRender( void );

	virtual void UpdateView( void );
	virtual void UpdateTotal( void );
};

#endif // BASEWORLDCAMERA_H