#ifndef BASEPORTALCAMERA_H
#define BASEPORTALCAMERA_H

#include "Global.h"
#include "BaseCopyCamera.h"

class CBasePortalCamera : public CBaseCopyCamera
{
public:
	DECLARE_CLASS( CBasePortalCamera, CBaseCopyCamera )

	CBasePortalCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

protected:
	virtual bool ShouldFlipPortal( void );

	virtual bool ShouldUpdateTransform( void );
	virtual void UpdateTransform( void );

protected:
	glm::vec4 m_vec4Plane;
	glm::mat4 m_matTransform;
};

#endif // BASEPORTALCAMERA_H