#ifndef CSMLIGHT_H
#define CSMLIGHT_H

#include "Global.h"
#include "BaseLight.h"
#include "CSMShadowCamera.h"

class CCSMLight : public CBaseLight
{
public:
	DECLARE_CLASS( CCSMLight, CBaseLight );

	CCSMLight( CCSMShadowCamera *pCSMShadowCamera, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular );

	virtual void ActivateLight( void );
};

#endif // CSMLIGHT_H