#ifndef ORTHOPLAYERCAMERA_H
#define ORTHOPLAYERCAMERA_H

#include "Global.h"
#include "BasePlayerCamera.h"

class COrthoPlayerCamera : public CBasePlayerCamera
{
public:
	DECLARE_CLASS( COrthoPlayerCamera, CBasePlayerCamera )

	DECLARE_LINKED_CLASS()

	COrthoPlayerCamera();

protected:
	virtual bool ShouldUpdateProjection( void ) const;

	virtual glm::mat4 CalculateProjection( void ) const;
};

#endif // ORTHOPLAYERCAMERA_H