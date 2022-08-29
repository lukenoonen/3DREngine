#ifndef CSMLIGHT_H
#define CSMLIGHT_H

#include "Global.h"
#include "BaseLight.h"
#include "CSMShadowCamera.h"

class CCSMLight : public CBaseLight
{
public:
	DECLARE_CLASS( CCSMLight, CBaseLight );

	CCSMLight();

	void SetShadowCamera( CCSMShadowCamera *pCSMShadowCamera );

	virtual void ActivateLight( void );
};

#endif // CSMLIGHT_H