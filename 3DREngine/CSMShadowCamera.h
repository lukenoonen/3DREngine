#ifndef CSMSHADOWCAMERA_H
#define CSMSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CCSMShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CCSMShadowCamera, CBaseShadowCamera );

	CCSMShadowCamera( float flBlendDistance, float flDistanceFactor, float flInitialDistance, float flNearZError, float flFarZError, float flFadeNear, float flFadeFar, float flBlurRadius, unsigned int uiBaseSize, unsigned int uiRenderPriority );
	virtual ~CCSMShadowCamera();

	virtual void PostThink( void );

	virtual void Render( void );

	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

	virtual void ActivateLight( void );

private:
	void CalculateCascade( void );
	void CalculateRadius( void );
	void CalculateTotal( void );
	void CalculateNearFar( void );

private:
	float m_flBlendDistance;
	float m_flDistanceFactor;
	float m_flInitialDistance;
	float m_flNearZError;
	float m_flFarZError;

	float m_flBlurRadius;

	glm::mat4 m_matTotal[4];
	float m_flCascadeEnd[5];
	float m_flCascadeEndNear[4];
	float m_flRadius[4];
	glm::vec4 m_vecCascadeEndClipSpaceNear;
	glm::vec4 m_vecCascadeEndClipSpaceFar;

	unsigned int m_uiTextureFBO;
	unsigned int m_uiTexture;
};

#endif // CSMSHADOWCAMERA_H