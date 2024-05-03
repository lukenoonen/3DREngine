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

protected:
	virtual bool ShouldFlipPortal( void );

	virtual glm::mat4 CalculateTransform( void ) const;
};

#endif // PLANARREFLECTIONCAMERA_H