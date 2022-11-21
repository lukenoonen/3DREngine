#ifndef BASEVARIABLESIZECAMERA_H
#define BASEVARIABLESIZECAMERA_H

#include "Global.h"
#include "BaseSizeCamera.h"

class CBaseVariableSizeCamera : public CBaseSizeCamera
{
public:
	DECLARE_CLASS( CBaseVariableSizeCamera, CBaseSizeCamera )

	DECLARE_DATADESC()

	CBaseVariableSizeCamera();

	virtual bool Init( void );

	virtual void PostThink( void );

	void SetBaseSize( unsigned int uiBaseSize );

	void SetSizeRatio( float flSizeRatio );

protected:
	unsigned int GetBaseSize( void ) const;

	float GetSizeRatio( void ) const;

private:
	unsigned int m_uiBaseSize;
	float m_flSizeRatio;

	bool m_bUpdateSize;
};

#endif // BASEVARIABLESIZECAMERA_H