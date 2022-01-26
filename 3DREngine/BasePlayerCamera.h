#ifndef BASEPLAYERCAMERA_H
#define BASEPLAYERCAMERA_H

#include "Global.h"
#include "BaseUniformMSAACamera.h"

class CBasePlayerCamera : public CBaseUniformMSAACamera
{
public:
	DECLARE_CLASS( CBasePlayerCamera, CBaseUniformMSAACamera );

	CBasePlayerCamera();

	virtual bool ShouldDraw( void ) const;

	void SetPlayerCamera( bool bPlayerCamera );

private:
	bool m_bPlayerCamera;
};

#endif // BASEPLAYERCAMERA_H