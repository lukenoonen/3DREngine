#ifndef BASECOPYMSAACAMERA_H
#define BASECOPYMSAACAMERA_H

#include "Global.h"
#include "BaseMSAACamera.h"
#include "Handle.h"

class CBaseCopyMSAACamera : public CBaseMSAACamera // TODO: consider trying to make this a BaseWorldCamera derived class
{
public:
	DECLARE_CLASS( CBaseCopyMSAACamera, CBaseMSAACamera )

	CBaseCopyMSAACamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	void SetTargetCamera( CBaseMSAACamera *pTargetCamera );

	void SetSizeQualityFactor( float flSizeQualityFactor );

	void SetMSAALevelQualityFactor( float flMSAALevelQualityFactor );

	CBaseMSAACamera *GetTargetCamera( void );

private:
	CHandle<CBaseMSAACamera> m_hTargetCamera;

	float m_flSizeQualityFactor;
	float m_flMSAALevelQualityFactor;

	glm::ivec2 m_vec2TargetCameraSize;
	unsigned char m_ucTargetCameraMSAALevel;

	bool m_bUpdateSize;
	bool m_bUpdateMSAALevel;
};

#endif // BASECOPYMSAACAMERA_H