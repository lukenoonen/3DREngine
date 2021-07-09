#ifndef SPOTLIGHTSHADOW_H
#define SPOTLIGHTSHADOW_H

#include "Global.h"
#include "SpotLight.h"

class CSpotLightShadow : public CSpotLight
{
public:
	DECLARE_CLASS( CSpotLightShadow, CSpotLight );

	CSpotLightShadow( float flBlurRadius, float flCutoff, float flOuterCutoff, float flConstant, float flLinear, float flQuadratic, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CSpotLightShadow();

	virtual bool CastShadows( void ) const;

	virtual void PostThink();

	virtual void CalculateShadows( void );
	virtual void ActivateLight( void );

	void CreateShadowBuffers( void );
	void DestroyShadowBuffers( void );

private:
	unsigned int m_uiShadowMapFBO;
	unsigned int m_uiShadowMap;

	float m_flBlurScale;

	glm::mat4 m_matProjectionMatrix;
	glm::mat4 m_matLightSpaceMatrix;
};

#endif // SPOTLIGHTSHADOW_H