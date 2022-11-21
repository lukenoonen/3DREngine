#ifndef PERSPECTIVEPLAYERCAMERA_H
#define PERSPECTIVEPLAYERCAMERA_H

#include "Global.h"
#include "BasePlayerCamera.h"

class CPerspectivePlayerCamera : public CBasePlayerCamera
{
public:
	DECLARE_CLASS( CPerspectivePlayerCamera, CBasePlayerCamera )

	DECLARE_LINKED_CLASS()

	CPerspectivePlayerCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

private:
	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matTotal;
	glm::mat4 m_matTotalLocked;
};

#endif // PERSPECTIVEPLAYERCAMERA_H