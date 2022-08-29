#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include "Global.h"
#include "BaseLight.h"
#include "DirShadowCamera.h"

class CDirLight : public CBaseLight
{
public:
	DECLARE_CLASS( CDirLight, CBaseLight )

	CDirLight();

	void SetShadowCamera( CDirShadowCamera *pDirShadowCamera );

	virtual void ActivateLight( void );
};

#endif // DIRLIGHT_H