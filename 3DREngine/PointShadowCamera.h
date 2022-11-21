#ifndef POINTSHADOWCAMERA_H
#define POINTSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CPointShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CPointShadowCamera, CBaseShadowCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CPointShadowCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual void ActivateLight( void );

	virtual int BindTexture( void );

	void SetNear( float flNear );

	void SetFar( float flFar );

	void SetBlurRadius( float flBlurRadius );

protected:
	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

private:
	float m_flNear;
	float m_flFar;

	glm::mat4 m_matProjection;
	glm::mat4 m_matView[6];
	glm::mat4 m_matTotal[6];

	bool m_bUpdateProjection;

	GLuint m_glTextureFBO;
	GLuint m_glTexture;
};

#endif // POINTSHADOWCAMERA_H