#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "Global.h"
#include "BaseTransform.h"
#include "BaseShadowCamera.h"

class CBaseLight : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseLight, CBaseTransform );

	CBaseLight( CBaseShadowCamera *pShadowCamera, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular );

	virtual bool IsLight( void ) const;

	virtual void ActivateLight( void );

private:
	CBaseShadowCamera *m_pShadowCamera;

	glm::vec3 m_vecAmbient;
	glm::vec3 m_vecDiffuse;
	glm::vec3 m_vecSpecular;
};

#endif // BASELIGHT_H