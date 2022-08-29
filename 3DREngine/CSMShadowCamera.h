#ifndef CSMSHADOWCAMERA_H
#define CSMSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CCSMShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CCSMShadowCamera, CBaseShadowCamera )

	CCSMShadowCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual void ActivateLight( void );

	virtual int BindTexture( void );

	void SetBlendDistance( float flBlendDistance );

	void SetDistanceFactor( float flDistanceFactor );

	void SetInitialDistance( float flInitialDistance );

	void SetNearError( float flNearError );

	void SetFarError( float flFarError );

	void SetBlurRadius( float flBlurRadius );

protected:
	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

private:
	void CalculateCascade( void );
	void CalculateRadius( void );
	void CalculateTotal( void );
	void CalculateNearFar( void );

private:
	float m_flBlendDistance;
	float m_flDistanceFactor;
	float m_flInitialDistance;
	float m_flNearError;
	float m_flFarError;
	float m_flBlurRadius;

	glm::mat4 m_matTotal[4];
	float m_flCascadeEnd[5];
	float m_flCascadeEndNear[4];
	float m_flRadius[4];
	glm::vec4 m_vec4CascadeEndClipSpaceNear;
	glm::vec4 m_vec4CascadeEndClipSpaceFar;

	bool m_bUpdateCascade;
	bool m_bUpdateRadius;
	bool m_bUpdateTotal;
	bool m_bUpdateNearFar;
	bool m_bUpdateBlurScale;

	GLuint m_glTextureFBO;
	GLuint m_glTexture;
};

#endif // CSMSHADOWCAMERA_H