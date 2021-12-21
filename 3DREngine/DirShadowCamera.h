#ifndef DIRSHADOWCAMERA_H
#define DIRSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CDirShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CDirShadowCamera, CBaseShadowCamera );

	CDirShadowCamera( float flLength, float flZNear, float flZFar, float flFadeNear, float flFadeFar, float flBlurRadius, unsigned int uiBaseSize, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CDirShadowCamera();

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

#endif // DIRSHADOWCAMERA_H