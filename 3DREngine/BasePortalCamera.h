#ifndef BASEPORTALCAMERA_H
#define BASEPORTALCAMERA_H

#include "Global.h"
#include "BaseCopyCamera.h"

class CBasePortalCamera : public CBaseCopyCamera
{
public:
	DECLARE_CLASS( CBasePortalCamera, CBaseCopyCamera )

	CBasePortalCamera();

	virtual bool Init( void );

	virtual void Think( void );

	virtual const glm::vec3 &GetCameraPosition( void ) const;
	virtual const glm::quat &GetCameraRotation( void ) const;

protected:
	virtual void PerformRender( void );

	virtual bool ShouldFlipPortal( void );

	virtual bool ShouldUpdateView( void ) const;
	virtual void UpdateView( void );

	virtual bool ShouldUpdateProjection( void ) const;

	virtual glm::mat4 CalculateView( void ) const;
	virtual glm::mat4 CalculateProjection( void ) const;

	virtual bool ShouldUpdateTransform( void ) const;
	virtual void UpdateTransform( void );

	virtual glm::mat4 CalculateTransform( void ) const = 0;

protected:
	glm::vec4 m_vec4Plane;
	glm::mat4 m_matTransform;

	glm::vec3 m_vec3CameraPosition;
	glm::quat m_qCameraRotation;
};

#endif // BASEPORTALCAMERA_H