#ifndef PLANARREFLECTIONCAMERA_H
#define PLANARREFLECTIONCAMERA_H

#include "Global.h"
#include "BaseReflectionCamera.h"

class CPlanarReflectionCamera : public CBaseReflectionCamera
{
public:
	DECLARE_CLASS( CPlanarReflectionCamera, CBaseReflectionCamera );

	CPlanarReflectionCamera();

	virtual void Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual int BindTexture( void );

protected:
	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

private:
	glm::vec4 m_vec4Plane;

	glm::mat4 m_matReflection;
	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matTotal;
	glm::mat4 m_matTotalLocked;

	unsigned int m_uiTextureFBO;
	unsigned int m_uiTexture;
};

#endif // PLANARREFLECTIONCAMERA_H