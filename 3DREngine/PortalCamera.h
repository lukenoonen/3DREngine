#ifndef PORTALCAMERA_H
#define PORTALCAMERA_H

#include "Global.h"
#include "BasePortalCamera.h"

class CPortalCamera : public CBasePortalCamera
{
public:
	DECLARE_CLASS( CPortalCamera, CBasePortalCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CPortalCamera();

protected:
	virtual bool ShouldUpdateTransform( void ) const;

	virtual glm::mat4 CalculateTransform( void ) const;

private:
	CHandle<CBaseTransform> m_hDisplay;
};

#endif // PORTALCAMERA_H