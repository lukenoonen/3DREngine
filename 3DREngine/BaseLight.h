#ifndef BASELIGHT_H
#define BASELIGHT_H

#include "Global.h"
#include "BaseTransform.h"
#include "BaseShadowCamera.h"

class CBaseLight : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseLight, CBaseTransform )

	DECLARE_DATADESC()

	CBaseLight();

	virtual bool IsLight( void ) const;

	virtual void PostThink( void );

	virtual void ActivateLight( void );

	void SetAmbient( const glm::vec3 &vec3Ambient );
	void SetDiffuse( const glm::vec3 &vec3Diffuse );
	void SetSpecular( const glm::vec3 &vec3Specular );

protected:
	virtual void CalculateMaxRadius( void );

	float GetMaxDiffuse( void ) const;

	CBaseShadowCamera *GetShadowCamera( void ) const;

private:
	CHandle<CBaseShadowCamera> m_hShadowCamera;

	glm::vec3 m_vec3Ambient;
	glm::vec3 m_vec3Diffuse;
	glm::vec3 m_vec3Specular;
};

#endif // BASELIGHT_H