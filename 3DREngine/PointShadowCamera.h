#ifndef POINTSHADOWCAMERA_H
#define POINTSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CPointShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CPointShadowCamera, CBaseShadowCamera );

	CPointShadowCamera( float flZNear, float flZFar, float flFadeNear, float flFadeFar, float flBlurRadius, unsigned int uiBaseSize, unsigned int uiRenderPriority );
	virtual ~CPointShadowCamera();

	virtual void PostThink( void );

	virtual void Render( void );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

	virtual void ActivateLight( void );

private:
	float m_flMaxRadius;

	glm::mat4 m_matProjection;
	glm::mat4 m_matTotal[6];

	unsigned int m_uiTextureFBO;
	unsigned int m_uiTexture;
};

#endif // POINTSHADOWCAMERA_H