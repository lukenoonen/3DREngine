#ifndef CAMERAANCHOR_H
#define CAMERAANCHOR_H

#include "Global.h"
#include "BaseTransform.h"
#include "BaseCamera.h"
#include "2DAlign.h"

class CCameraAnchor : public CBaseTransform
{
public:
	DECLARE_CLASS( CCameraAnchor, CBaseTransform )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CCameraAnchor();

	virtual bool Init( void );

	virtual void PostThink( void );

private:
	void Anchor( void );

private:
	CHandle<CBaseCamera> m_hCamera;

	E2DAlign m_e2DAlign;
	float m_flDepth;

	glm::quat m_qInverseCamera;
};

#endif // CAMERAANCHOR_H