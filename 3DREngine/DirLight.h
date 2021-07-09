#ifndef DIRLIGHT_H
#define DIRLIGHT_H

#include "Global.h"
#include "BaseLight.h"

class CDirLight : public CBaseLight
{
public:
	DECLARE_CLASS( CDirLight, CBaseLight );

	CDirLight( const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void ActivateLight( void );
};

#endif // DIRLIGHT_H