#ifndef BASECOPYCAMERA_H
#define BASECOPYCAMERA_H

#include "BaseWorldCamera.h"

class CBaseCopyCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( CBaseCopyCamera, CBaseWorldCamera )

	DECLARE_DATADESC()

	CBaseCopyCamera();

	virtual bool Init( void );

	virtual void Think( void );

protected:
	CBaseWorldCamera *GetTargetCamera( void ) const;

private:
	CHandle<CBaseWorldCamera> m_hTargetCamera;

	CMonitoredValue<float> m_flSizeQualityFactor;
	CMonitoredValue<float> m_flMSAALevelQualityFactor;

	glm::ivec2 m_vec2TargetSize;
	unsigned char m_ucTargetMSAALevel;
};

#endif // BASECOPYCAMERA_H