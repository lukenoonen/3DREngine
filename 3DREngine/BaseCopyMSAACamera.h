#ifndef BASECOPYMSAACAMERA_H
#define BASECOPYMSAACAMERA_H

#include "Global.h"
#include "BaseMSAACamera.h"

class CBaseCopyMSAACamera : public CBaseMSAACamera // TODO: consider trying to make this a BaseWorldCamera derived class
{
public:
	DECLARE_CLASS( CBaseCopyMSAACamera, CBaseMSAACamera );

	CBaseCopyMSAACamera();

	virtual void Init( void );

	virtual void PostThink( void );

	void SetTargetCamera( CBaseMSAACamera *pTargetCamera );

	void SetSizeQualityFactor( float flSizeQualityFactor );

	void SetMSAALevelQualityFactor( float flMSAALevelQualityFactor );

	CBaseMSAACamera *GetTargetCamera( void );

private:
	CBaseMSAACamera *m_pTargetCamera;

	float m_flSizeQualityFactor;
	float m_flMSAALevelQualityFactor;

	glm::ivec2 m_vec2TargetCameraSize;
	unsigned int m_uiTargetCameraMSAALevel;

	bool m_bUpdateSize;
	bool m_bUpdateMSAALevel;
};

#endif // BASECOPYMSAACAMERA_H