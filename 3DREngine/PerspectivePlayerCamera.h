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

protected:
	virtual bool ShouldUpdateProjection( void ) const;

	virtual glm::mat4 CalculateProjection( void ) const;
};

#endif // PERSPECTIVEPLAYERCAMERA_H