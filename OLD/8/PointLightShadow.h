#ifndef POINTLIGHTSHADOW_H
#define POINTLIGHTSHADOW_H

#include "Global.h"
#include "PointLight.h"

class CPointLightShadow : public CPointLight
{
public:
	DECLARE_CLASS( CPointLightShadow, CPointLight );

	CPointLightShadow( float flFadeNear, float flFadeFar, float flBlurRadius, float flConstant, float flLinear, float flQuadratic, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CPointLightShadow();

	virtual void PostThink( void );

	virtual void CalculateShadows( void );
	virtual void ActivateLight( void );

private:
	unsigned int m_uiShadowMapFBO;
	unsigned int m_uiShadowMap;

	float m_flFadeNear;
	float m_flFadeFar;
	float m_flBlurScale;

	glm::mat4 m_matProjectionMatrix;
	glm::mat4 m_matLightSpaceMatricies[6];
};

#endif // POINTLIGHTSHADOW_H