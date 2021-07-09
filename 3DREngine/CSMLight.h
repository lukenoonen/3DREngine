#ifndef CSMLIGHT_H
#define CSMLIGHT_H

#include "Global.h"
#include "BaseLight.h"

class CCSMLight : public CBaseLight
{
public:
	DECLARE_CLASS( CCSMLight, CBaseLight );

	CCSMLight( float flBlurRadius, float flBlendDistance, float flInitialDistance, float flDistanceFactor, float flFarZError, float flNearZError, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CCSMLight();

	virtual bool CastShadows( void ) const;

	virtual void CalculateShadows( void );
	virtual void ActivateLight( void );

	void CreateShadowBuffers( void );
	void DestroyShadowBuffers( void );

private:
	unsigned int m_uiShadowMapFBOs[4];
	unsigned int m_uiShadowMaps[4];
	glm::mat4 m_matLightSpaceMatricies[4];
	float m_flCascadeEnd[5];
	float m_flCascadeEndNear[4];
	glm::vec4 m_vecCascadeEndClipSpaceNear;
	glm::vec4 m_vecCascadeEndClipSpaceFar;
	glm::vec4 m_vecCascadeBlurScale;
	float m_flFarZError;
	float m_flNearZError;
	float m_flBlurRadius;
};

#endif // CSMLIGHT_H