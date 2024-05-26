#ifndef POINTSHADOWCAMERA_H
#define POINTSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"
#include "FramebufferShadowCubemap.h"

class CPointShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CPointShadowCamera, CBaseShadowCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CPointShadowCamera();

	virtual void ActivateLight( void );

	virtual CBaseFramebuffer *GetFramebuffer( void );
	virtual const CBaseFramebuffer *GetFramebuffer( void ) const;

	void SetNear( float flNear );
	void SetFar( float flFar );
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
	CFramebufferShadowCubemap m_fFramebufferShadowCubemap;

	CMonitoredValue<float> m_flNear;
	CMonitoredValue<float> m_flFar;

	glm::mat4 m_matView[6];
	glm::mat4 m_matProjection;
	glm::mat4 m_matTotal[6];
};

#endif // POINTSHADOWCAMERA_H