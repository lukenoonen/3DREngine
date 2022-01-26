#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "Global.h"
#include "BaseTransform.h"
#include "BaseShadowCamera.h"

class CBaseLight : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseLight, CBaseTransform );

	CBaseLight();

	virtual bool IsLight( void ) const;

	virtual void ActivateLight( void );

	void SetAmbient( const glm::vec3 &vecAmbient );
	void SetDiffuse( const glm::vec3 &vecDiffuse );
	void SetSpecular( const glm::vec3 &vecSpecular );

protected:
	void SetShadowCamera( CBaseShadowCamera *pShadowCamera );

	virtual void CalculateMaxRadius( void );

	float GetMaxDiffuse( void ) const;

private:
	CBaseShadowCamera *m_pShadowCamera;

	glm::vec3 m_vecAmbient;
	glm::vec3 m_vecDiffuse;
	glm::vec3 m_vecSpecular;
};

#endif // BASELIGHT_H