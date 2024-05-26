#ifndef DIRSHADOWCAMERA_H
#define DIRSHADOWCAMERA_H

#include "Global.h"
#include "BaseShadowCamera.h"
#include "FramebufferShadow.h"

class CDirShadowCamera : public CBaseShadowCamera
{
public:
	DECLARE_CLASS( CDirShadowCamera, CBaseShadowCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CDirShadowCamera();

	virtual bool Init( void );

	virtual void Think( void );

	virtual void ActivateLight( void );

	virtual CBaseFramebuffer *GetFramebuffer( void );
	virtual const CBaseFramebuffer *GetFramebuffer( void ) const;

	void SetWidth( float flWidth );
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
	CFramebufferShadow m_fFramebufferShadow;

	CMonitoredValue<float> m_flWidth;
	CMonitoredValue<float> m_flNear;
	CMonitoredValue<float> m_flFar;
	CMonitoredValue<float> m_flBlurRadius;

	glm::mat4 m_matView;
	glm::mat4 m_matProjection;
	glm::mat4 m_matTotal;
};

#endif // DIRSHADOWCAMERA_H