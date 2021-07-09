#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Global.h"
#include "PointLight.h"

class CSpotLight : public CPointLight
{
public:
	DECLARE_CLASS( CSpotLight, CPointLight );

	CSpotLight( float flCutoff, float flOuterCutoff, float flConstant, float flLinear, float flQuadratic, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void ActivateLight( void );

private:
	float m_flCutoff;
	float m_flOuterCutoff;
};

#endif // SPOTLIGHT_H