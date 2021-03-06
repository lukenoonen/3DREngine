#ifndef BASEMSAACAMERA_H
#define BASEMSAACAMERA_H

#include "Global.h"
#include "BaseSizeCamera.h"

class CBaseMSAACamera : public CBaseSizeCamera
{
public:
	DECLARE_CLASS( CBaseMSAACamera, CBaseSizeCamera );

	CBaseMSAACamera();

	void SetMSAALevel( unsigned int uiMSAALevel );

	unsigned int GetMSAALevel( void ) const;
	
protected:
	unsigned int GetMSAAFBO( void ) const;

	virtual void CreateMSAABuffers( void );
	virtual void DestroyMSAABuffers( void );

private:
	unsigned int m_uiMSAALevel;

	unsigned int m_uiMSAAFBO;
	unsigned int m_uiMSAA;
	unsigned int m_uiMSAARBO;
};

#endif // BASEMSAACAMERA_H