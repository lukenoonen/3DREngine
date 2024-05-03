#ifndef BASEWORLDCAMERA_H
#define BASEWORLDCAMERA_H

#include "Global.h"
#include "BaseCamera.h"

class CBasePortalCamera;

class CBaseWorldCamera : public CBaseCamera
{
public:
	DECLARE_CLASS( CBaseWorldCamera, CBaseCamera )

	CBaseWorldCamera();

	friend class CBasePortalCamera;

	virtual const glm::mat4 &GetView( void ) const;
	virtual const glm::mat4 &GetProjection( void ) const;
	virtual const glm::mat4 &GetTotal( void ) const;

protected:
	virtual void PerformRender( void );

	virtual bool ShouldUpdateView( void ) const;
	virtual void UpdateView( void );

	virtual void UpdateProjection( void );

	virtual void UpdateTotal( void );

	virtual glm::mat4 CalculateView( void ) const;
	virtual glm::mat4 CalculateProjection( void ) const = 0;

private:
	glm::mat4 m_matView;
	glm::mat4 m_matProjection;
	glm::mat4 m_matTotal;
	glm::mat4 m_matTotalLocked;
};

#endif // BASEWORLDCAMERA_H