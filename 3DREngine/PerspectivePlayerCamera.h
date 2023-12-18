#ifndef PERSPECTIVEPLAYERCAMERA_H
#define PERSPECTIVEPLAYERCAMERA_H

#include "Global.h"
#include "BasePlayerCamera.h"

class CPerspectivePlayerCamera : public CBasePlayerCamera
{
public:
	DECLARE_CLASS( CPerspectivePlayerCamera, CBasePlayerCamera )

	DECLARE_LINKED_CLASS()

	CPerspectivePlayerCamera();

	virtual void PostThink( void );

protected:
	virtual void UpdateProjection( void );
};

#endif // PERSPECTIVEPLAYERCAMERA_H