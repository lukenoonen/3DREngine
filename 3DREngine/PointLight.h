#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Global.h"
#include "BaseLight.h"
#include "PointShadowCamera.h"

class CPointLight : public CBaseLight
{
public:
	DECLARE_CLASS( CPointLight, CBaseLight );

	CPointLight( float flConstant, float flLinear, float flQuadratic, CPointShadowCamera *pPointShadowCamera, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular );

	virtual void ActivateLight( void );

private:
	float m_flConstant;
	float m_flLinear;
	float m_flQuadratic;

	float m_flMaxRadius;
};

#endif // POINTLIGHT_H