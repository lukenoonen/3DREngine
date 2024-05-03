#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "Global.h"
#include "BaseTransform.h"
#include "BaseFramebuffer.h"

// TODO: clean up the use of Init() in these classes and derrived classes, since they often call the same functions as PostThink()

class CCameraAnchor;

class CBaseCamera : public CBaseTransform
{
public:
	friend class CCameraAnchor;

	DECLARE_CLASS( CBaseCamera, CBaseTransform )

	DECLARE_DATADESC()

	CBaseCamera();
	virtual ~CBaseCamera();

	virtual bool Init( void );

	virtual void Think( void );

	virtual bool IsCamera( void ) const;

	void Render( void );

	int Bind( void );

	int GetPriority( void ) const;

	void InitFramebuffer( CBaseFramebuffer *pFramebuffer );
	virtual CBaseFramebuffer *GetFramebuffer( void ) const;

	virtual const glm::vec3 &GetCameraPosition( void ) const;
	virtual const glm::quat &GetCameraRotation( void ) const;

	virtual const glm::mat4 &GetView( void ) const = 0;
	virtual const glm::mat4 &GetProjection( void ) const = 0;
	virtual const glm::mat4 &GetTotal( void ) const = 0;

protected:
	virtual void PerformRender( void ) = 0;

	virtual bool ShouldUpdateView( void ) const = 0;
	virtual void UpdateView( void ) = 0;

	virtual bool ShouldUpdateProjection( void ) const = 0;
	virtual void UpdateProjection( void ) = 0;

	virtual void UpdateTotal( void ) = 0;

protected:
	CBaseFramebuffer *m_pFramebuffer;

private:
	int m_iPriority;
};

#endif // BASECAMERA_H