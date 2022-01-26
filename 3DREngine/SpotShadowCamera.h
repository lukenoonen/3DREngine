#ifndef SPOTSHADOWCAMERA_H
#define SPOTSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CSpotShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CSpotShadowCamera, CBaseShadowCamera );

	CSpotShadowCamera();

	virtual void Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual void ActivateLight( void );

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

	unsigned int m_uiTextureFBO;
	unsigned int m_uiTexture;
};

#endif // SPOTSHADOWCAMERA_H