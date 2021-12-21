#ifndef PERSPECTIVEWORLDCAMERA_H
#define PERSPECTIVEWORLDCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"

class CPerspectiveWorldCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( CPerspectiveWorldCamera, CBaseWorldCamera );

	CPerspectiveWorldCamera( float flFOV, float flNear, float flFar, unsigned int uiBaseSize, float flSizeRatio, unsigned int uiBaseMSAALevel, unsigned int uiRenderPriority, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CPerspectiveWorldCamera();

	virtual void PostThink( void );
	virtual void Render( void );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

private:
	glm::mat4 m_matProjection;
	glm::mat4 m_matTotal;
	glm::mat4 m_matTotalLocked;

	unsigned int m_uiTextureFBO;
	unsigned int m_uiTexture;
};

#endif // PERSPECTIVEWORLDCAMERA_H