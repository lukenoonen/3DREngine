#ifndef SPOTSHADOWCAMERA_H
#define SPOTSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CSpotShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CSpotShadowCamera, CBaseShadowCamera )

	CSpotShadowCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual void ActivateLight( void );

	virtual int BindTexture( void );

	void SetNear( float flNear );

	void SetFar( float flFar );

	void SetOuterCutoff( float flOuterCutoff );

	void SetBlurRadius( float flBlurRadius );

protected:
	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

private:
	float m_flNear;
	float m_flFar;
	float m_flOuterCutoff;
	float m_flBlurRadius;

	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matTotal;

	bool m_bUpdateProjection;
	bool m_bUpdateBlurScale;

	GLuint m_glTextureFBO;
	GLuint m_glTexture;
};

#endif // SPOTSHADOWCAMERA_H