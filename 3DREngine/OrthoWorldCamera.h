#ifndef OrthoWORLDCAMERA_H
#define OrthoWORLDCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"

class COrthoWorldCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( COrthoWorldCamera, CBaseWorldCamera );

	COrthoWorldCamera( float flHeight, float flNear, float flFar, unsigned int uiBaseSize, float flSizeRatio, unsigned int uiBaseMSAALevel, unsigned int uiRenderPriority );
	virtual ~COrthoWorldCamera();

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

#endif // OrthoWORLDCAMERA_H