#ifndef CSMSHADOWCAMERA_H
#define CSMSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"
#include "FramebufferShadowCSM.h"

class CCSMShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CCSMShadowCamera, CBaseShadowCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CCSMShadowCamera();

	virtual bool Init( void );

	virtual void Think( void );

	virtual void ActivateLight( void );

	virtual CBaseFramebuffer *GetFramebuffer( void );
	virtual const CBaseFramebuffer *GetFramebuffer( void ) const;

	void SetBlendDistance( float flBlendDistance );
	void SetDistanceFactor( float flDistanceFactor );
	void SetInitialDistance( float flInitialDistance );
	void SetNearError( float flNearError );
	void SetFarError( float flFarError );
	void SetBlurRadius( float flBlurRadius );

	virtual const glm::mat4 &GetView( void ) const;
	virtual const glm::mat4 &GetProjection( void ) const;
	virtual const glm::mat4 &GetTotal( void ) const;

protected:
	virtual void PerformRender( void );
	
	virtual void UpdateView( void );
	virtual void UpdateProjection( void );
	virtual void UpdateTotal( void );

	virtual bool ShouldUpdateView( void ) const;
	virtual bool ShouldUpdateProjection( void ) const;

private:
	void CalculateCascade( void );
	void CalculateRadius( void ); 
	void CalculateNearFar( void );
	void CalculateBlurScale( void );

private:
	CFramebufferShadowCSM m_fFramebufferShadowCSM;

	CMonitoredValue<float> m_flBlendDistance;
	CMonitoredValue<float> m_flDistanceFactor;
	CMonitoredValue<float> m_flInitialDistance;
	CMonitoredValue<float> m_flNearError;
	CMonitoredValue<float> m_flFarError;
	CMonitoredValue<float> m_flBlurRadius;

	bool m_bUpdateProjection;

	float m_flCascadeEnd[5];
	float m_flCascadeEndNear[4];
	float m_flRadius[4];
	glm::vec4 m_vec4CascadeEndClipSpaceNear;
	glm::vec4 m_vec4CascadeEndClipSpaceFar;

	glm::mat4 m_matView;
	glm::mat4 m_matProjection[4];
	glm::mat4 m_matTotal[4];
};

#endif // CSMSHADOWCAMERA_H