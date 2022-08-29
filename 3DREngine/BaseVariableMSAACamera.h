#ifndef BASEVARIABLEMSAACAMERA_H
#define BASEVARIABLEMSAACAMERA_H

#include "Global.h"
#include "BaseMSAACamera.h"

class CBaseVariableMSAACamera : public CBaseMSAACamera
{
public:
	DECLARE_CLASS( CBaseVariableMSAACamera, CBaseMSAACamera )

	CBaseVariableMSAACamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	void SetBaseSize( unsigned int uiBaseSize );

	void SetSizeRatio( float flSizeRatio );

	void SetBaseMSAALevel( unsigned char ucBaseMSAALevel );

protected:
	float GetSizeRatio( void ) const;

private:
	unsigned int m_uiBaseSize;
	float m_flSizeRatio;

	unsigned char m_ucBaseMSAALevel;

	bool m_bUpdateSize;
	bool m_bUpdateMSAA;
};

#endif // BASEVARIABLEMSAACAMERA_H