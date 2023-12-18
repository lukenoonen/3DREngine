#ifndef BASECAMERA_H
#define BASECAMERA_H

#include "Global.h"
#include "BaseTransform.h"
#include "BaseFramebuffer.h"

// TODO: clean up the use of Init() in these classes and derrived classes, since they often call the same functions as PostThink()

class CBaseCamera : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseCamera, CBaseTransform )

	DECLARE_DATADESC()

	CBaseCamera();
	virtual ~CBaseCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	virtual bool IsCamera( void ) const;

	void Render( void );

	int Bind( void );

	int GetPriority( void ) const;

protected:
	virtual void PerformRender( void );

	virtual void UpdateView( void );
	virtual void UpdateProjection( void );
	virtual void UpdateTotal( void );

	virtual bool ShouldUpdateView( void );
	virtual bool ShouldUpdateProjection( void );

	void MarkUpdateProjection( void );

protected:
	CBaseFramebuffer *m_pFramebuffer;

	std::vector<glm::mat4> m_matView;
	std::vector<glm::mat4> m_matProjection;
	std::vector<glm::mat4> m_matTotal;
	std::vector<glm::mat4> m_matTotalLocked;

private:
	int m_iPriority;

	bool m_bUpdateProjection;
};

#endif // BASECAMERA_H