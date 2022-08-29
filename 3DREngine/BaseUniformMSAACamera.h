#ifndef BASEUNIFORMMSAACAMERA_H
#define BASEUNIFORMMSAACAMERA_H

#include "Global.h"
#include "BaseMSAACamera.h"

class CBaseUniformMSAACamera : public CBaseMSAACamera
{
public:
	DECLARE_CLASS( CBaseUniformMSAACamera, CBaseMSAACamera )

	CBaseUniformMSAACamera();

	virtual void PostThink( void );
};

#endif // BASEUNIFORMMSAACAMERA_H