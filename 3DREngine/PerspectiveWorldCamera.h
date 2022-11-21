#ifndef PERSPECTIVEWORLDCAMERA_H
#define PERSPECTIVEWORLDCAMERA_H

#include "Global.h"
#include "BaseWorldCamera.h"

class CPerspectiveWorldCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( CPerspectiveWorldCamera, CBaseWorldCamera )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CPerspectiveWorldCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual void Render( void );

	virtual int BindTexture( void );

	void SetFOV( float flFOV );

	void SetNear( float flNear );

	void SetFar( float flFar );

protected:
	virtual void CreateTextureBuffers( void );
	virtual void DestroyTextureBuffers( void );

private:
	float m_flFOV;
	float m_flNear;
	float m_flFar;

	glm::mat4 m_matProjection;
	glm::mat4 m_matView;
	glm::mat4 m_matTotal;
	glm::mat4 m_matTotalLocked;

	bool m_bUpdateProjection;

	GLuint m_glTextureFBO;
	GLuint m_glTexture;
};

#endif // PERSPECTIVEWORLDCAMERA_H