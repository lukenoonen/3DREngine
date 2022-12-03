#ifndef PORTALCAMERA_H
#define PORTALCAMERA_H

#include "Global.h"
#include "BaseReflectionCamera.h"

class CPortalCamera : public CBaseReflectionCamera
{
public:
	DECLARE_CLASS( CPortalCamera, CBaseReflectionCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CPortalCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual int BindTexture( void );

protected:
	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

private:
	glm::vec3 m_vec3DisplayPosition;
	glm::quat m_qDisplayRotation;
	glm::vec4 m_vec4Plane;

	glm::mat4 m_matReflection;
	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matTotal;
	glm::mat4 m_matTotalLocked;

	GLuint m_glTextureFBO;
	GLuint m_glTexture;
};

#endif // PORTALCAMERA_H