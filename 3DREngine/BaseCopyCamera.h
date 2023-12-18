#ifndef BASECOPYCAMERA_H
#define BASECOPYCAMERA_H

#include "BaseWorldCamera.h"

class CBaseCopyCamera : public CBaseWorldCamera
{
public:
	DECLARE_CLASS( CBaseCopyCamera, CBaseWorldCamera )

	DECLARE_DATADESC()

	CBaseCopyCamera();

	virtual bool Init( void );

protected:
	CBaseWorldCamera *GetTarget( void ) const;

	virtual void UpdateView( void );

private:
	CHandle<CBaseWorldCamera> m_hTarget;
};

#endif // BASECOPYCAMERA_H