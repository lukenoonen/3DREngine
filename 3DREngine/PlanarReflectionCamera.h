#ifndef PLANARREFLECTIONCAMERA_H
#define PLANARREFLECTIONCAMERA_H

#include "Global.h"
#include "BasePortalCamera.h"

class CPlanarReflectionCamera : public CBasePortalCamera
{
public:
	DECLARE_CLASS( CPlanarReflectionCamera, CBasePortalCamera )

	DECLARE_LINKED_CLASS()

	CPlanarReflectionCamera();

	virtual int BindTexture( void );

protected:
	virtual bool ShouldFlipPortal( void );

	virtual void UpdateTransform( void );
};

#endif // PLANARREFLECTIONCAMERA_H