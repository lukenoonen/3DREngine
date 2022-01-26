#ifndef BASEVARIABLEMSAACAMERA_H
#define BASEVARIABLEMSAACAMERA_H

#include "Global.h"
#include "BaseMSAACamera.h"

class CBaseVariableMSAACamera : public CBaseMSAACamera
{
public:
	DECLARE_CLASS( CBaseVariableMSAACamera, CBaseMSAACamera );

	CBaseVariableMSAACamera();

	virtual void Init( void );

	virtual void PostThink( void );

	void SetBaseSize( unsigned int uiBaseSize );

	void SetSizeRatio( float flSizeRatio );

	void SetBaseMSAALevel( unsigned int uiBaseMSAALevel );

protected:
	float GetSizeRatio( void ) const;

private:
	unsigned int m_uiBaseSize;
	float m_flSizeRatio;

	unsigned int m_uiBaseMSAALevel;

	bool m_bUpdateSize;
	bool m_bUpdateMSAA;
};

#endif // BASEVARIABLEMSAACAMERA_H