#ifndef DIRSHADOWCAMERA_H
#define DIRSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CDirShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CDirShadowCamera, CBaseShadowCamera )

	CDirShadowCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual void ActivateLight( void );

	virtual int BindTexture( void );

	void SetHeight( float flHeight );

	void SetNear( float flNear );

	void SetFar( float flFar );

	void SetBlurRadius( float flBlurRadius );

protected:
	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

private:
	float m_flHeight;
	float m_flNear;
	float m_flFar;
	float m_flBlurRadius;

	glm::mat4 m_matView;
	glm::mat4 m_matProjection;
	glm::mat4 m_matTotal;

	bool m_bUpdateProjection;
	bool m_bUpdateBlurScale;

	GLuint m_glTextureFBO;
	GLuint m_glTexture;
};

#endif // DIRSHADOWCAMERA_H