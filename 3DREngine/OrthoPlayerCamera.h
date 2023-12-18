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

	virtual void PostThink( void );

protected:
	virtual void UpdateProjection( void );
};

#endif // ORTHOPLAYERCAMERA_H