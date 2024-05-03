#ifndef SPOTSHADOWCAMERA_H
#define SPOTSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"

class CSpotShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CSpotShadowCamera, CBaseShadowCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CSpotShadowCamera();

	virtual bool Init( void );

	virtual void Think( void );

	virtual void ActivateLight( void );

	void SetNear( float flNear );
	void SetFar( float flFar );
	void SetOuterCutoff( float flOuterCutoff );
	void SetBlurRadius( float flBlurRadius );

	virtual const glm::mat4 &GetView( void ) const;
	virtual const glm::mat4 &GetProjection( void ) const;
	virtual const glm::mat4 &GetTotal( void ) const;

protected:
	virtual void PerformRender( void );

	virtual bool ShouldUpdateView( void ) const;
	virtual void UpdateView( void );

	virtual bool ShouldUpdateProjection( void ) const;
	virtual void UpdateProjection( void );

	virtual void UpdateTotal( void );

private:
	CMonitoredValue<float> m_flNear;
	CMonitoredValue<float> m_flFar;
	CMonitoredValue<float> m_flOuterCutoff;
	CMonitoredValue<float> m_flBlurRadius;

	glm::mat4 m_matView;
	glm::mat4 m_matProjection;
	glm::mat4 m_matTotal;
};

#endif // SPOTSHADOWCAMERA_H