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

	virtual int BindTexture( void );

protected:
	virtual bool ShouldUpdateTransform( void );
	virtual void UpdateTransform( void );

private:
	CHandle<CBaseTransform> m_hDisplay;
};

#endif // PORTALCAMERA_H