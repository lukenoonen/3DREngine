#ifndef SPOTSHADOWCAMERA_H
#define SPOTSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CSpotShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CSpotShadowCamera, CBaseShadowCamera );

	CSpotShadowCamera( float flOuterCutoff, float flZNear, float flZFar, float flFadeNear, float flFadeFar, float flBlurRadius, unsigned int uiBaseSize, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CSpotShadowCamera();

	virtual void PostThink( void );

	virtual void Render( void );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

	virtual void ActivateLight( void );

private:
	glm::mat4 m_matProjection;
	glm::mat4 m_matTotal;

	unsigned int m_uiTextureFBO;
	unsigned int m_uiTexture;
};

#endif // SPOTSHADOWCAMERA_H