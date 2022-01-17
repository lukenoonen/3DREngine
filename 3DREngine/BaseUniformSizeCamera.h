#ifndef BASEUNIFORMSIZECAMERA_H
#define BASEUNIFORMSIZECAMERA_H

#include "Global.h"
#include "BaseSizeCamera.h"

class CBaseUniformSizeCamera : public CBaseSizeCamera
{
public:
	DECLARE_CLASS( CBaseUniformSizeCamera, CBaseSizeCamera );

	CBaseUniformSizeCamera( unsigned int uiRenderPriority );

	virtual void PreThink( void );
};

#endif // BASEUNIFORMSIZECAMERA_H