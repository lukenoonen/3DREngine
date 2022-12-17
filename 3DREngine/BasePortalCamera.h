#ifndef BASEPORTALCAMERA_H
#define BASEPORTALCAMERA_H

#include "Global.h"
#include "BaseCopyMSAACamera.h"

class CBasePortalCamera : public CBaseCopyMSAACamera
{
public:
	DECLARE_CLASS( CBasePortalCamera, CBaseCopyMSAACamera )

	CBasePortalCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual int BindTexture( void );

protected:
	virtual bool ShouldFlipPortal( void );

	virtual bool ShouldUpdateTransform( void );
	virtual void UpdateTransform( void );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

protected:
	glm::vec4 m_vec4Plane;
	glm::mat4 m_matTransform;

private:

	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matTotal;
	glm::mat4 m_matTotalLocked;

	GLuint m_glTextureFBO;
	GLuint m_glTexture;
};

#endif // BASEPORTALCAMERA_H