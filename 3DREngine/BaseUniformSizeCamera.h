#ifndef BASEUNIFORMSIZECAMERA_H
#define BASEUNIFORMSIZECAMERA_H

#include "Global.h"
#include "BaseSizeCamera.h"

class CBaseUniformSizeCamera : public CBaseSizeCamera
{
public:
	DECLARE_CLASS( CBaseUniformSizeCamera, CBaseSizeCamera )

	CBaseUniformSizeCamera();

	virtual void PostThink( void );
};

#endif // BASEUNIFORMSIZECAMERA_H