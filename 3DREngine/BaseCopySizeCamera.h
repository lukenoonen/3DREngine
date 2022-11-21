#ifndef BASECOPYSIZECAMERA_H
#define BASECOPYSIZECAMERA_H

#include "Global.h"
#include "BaseSizeCamera.h"

class CBaseCopySizeCamera : public CBaseSizeCamera
{
public:
	DECLARE_CLASS( CBaseCopySizeCamera, CBaseSizeCamera )

	DECLARE_DATADESC()

	CBaseCopySizeCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	void SetTargetCamera( CBaseSizeCamera *pTargetCamera );
	void SetSizeQualityFactor( float flSizeQualityFactor );

	CBaseSizeCamera *GetTargetCamera( void );

private:
	CHandle<CBaseSizeCamera> m_hTargetCamera;

	float m_flSizeQualityFactor;

	glm::ivec2 m_vec2TargetCameraSize;

	bool m_bUpdateSize;
};

#endif // BASECOPYSIZECAMERA_H