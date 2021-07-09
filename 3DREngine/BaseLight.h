#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "Global.h"
#include "BaseTransform.h"

class CBaseLight : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseLight, CBaseTransform );

	CBaseLight( const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual bool IsLight( void ) const;

	virtual bool CastShadows( void ) const;

	virtual void CalculateShadows( void );
	virtual void ActivateLight( void );

private:
	glm::vec3 m_vecAmbient;
	glm::vec3 m_vecDiffuse;
	glm::vec3 m_vecSpecular;
};

#endif // BASELIGHT_H