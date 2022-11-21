#ifndef BASEMSAACAMERA_H
#define BASEMSAACAMERA_H

#include "Global.h"
#include "BaseSizeCamera.h"

class CBaseMSAACamera : public CBaseSizeCamera
{
public:
	DECLARE_CLASS( CBaseMSAACamera, CBaseSizeCamera )

	DECLARE_DATADESC()

	CBaseMSAACamera();

	void SetMSAALevel( unsigned char ucMSAALevel );

	// TODO: should this be public? In general, data protection
	unsigned char GetMSAALevel( void ) const;
	
protected:
	GLuint GetMSAAFBO( void ) const;

	virtual void CreateMSAABuffers( void );
	virtual void DestroyMSAABuffers( void );

private:
	unsigned char m_ucMSAALevel;

	GLuint m_glMSAAFBO;
	GLuint m_glMSAA;
	GLuint m_glMSAARBO;
};

#endif // BASEMSAACAMERA_H