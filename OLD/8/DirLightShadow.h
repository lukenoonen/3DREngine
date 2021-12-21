#ifndef DIRLIGHTSHADOW_H
#define DIRLIGHTSHADOW_H

#include "Global.h"
#include "DirLight.h"

class CDirLightShadow : public CDirLight
{
public:
	DECLARE_CLASS( CDirLightShadow, CDirLight );

	CDirLightShadow( float flFadeNear, float flFadeFar, float flBlurRadius, float flLength, float flZNear, float flZFar, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CDirLightShadow();

	virtual void PostThink( void );

	virtual void CalculateShadows( void );
	virtual void ActivateLight( void );

private:
	unsigned int m_uiShadowMapFBO;
	unsigned int m_uiShadowMap;

	float m_flFadeNear;
	float m_flFadeFar;
	float m_flBlurScale;

	glm::mat4 m_matOrthoMatrix;
	glm::mat4 m_matLightSpaceMatrix;
};

#endif // DIRLIGHTSHADOW_H