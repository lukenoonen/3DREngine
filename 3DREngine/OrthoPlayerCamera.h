#ifndef ORTHOPLAYERCAMERA_H
#define ORTHOPLAYERCAMERA_H

#include "Global.h"
#include "BasePlayerCamera.h"

class COrthoPlayerCamera : public CBasePlayerCamera
{
public:
	DECLARE_CLASS( COrthoPlayerCamera, CBasePlayerCamera )

	DECLARE_LINKED_CLASS()

	COrthoPlayerCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

private:
	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matTotal;
	glm::mat4 m_matTotalLocked;
};

#endif // ORTHOPLAYERCAMERA_H