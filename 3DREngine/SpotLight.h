#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Global.h"
#include "BaseLight.h"
#include "SpotShadowCamera.h"

class CSpotLight : public CBaseLight
{
public:
	DECLARE_CLASS( CSpotLight, CBaseLight );

	CSpotLight( float flCutoff, float flOuterCutoff, float flConstant, float flLinear, float flQuadratic, CSpotShadowCamera *pSpotShadowCamera, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular );

	virtual void ActivateLight( void );

private:
	float m_flConstant;
	float m_flLinear;
	float m_flQuadratic;

	float m_flMaxRadius;

	float m_flCutoff;
	float m_flOuterCutoff;
};

#endif // SPOTLIGHT_H