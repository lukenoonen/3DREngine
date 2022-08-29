#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Global.h"
#include "BaseLight.h"
#include "PointShadowCamera.h"

class CPointLight : public CBaseLight
{
public:
	DECLARE_CLASS( CPointLight, CBaseLight )

	CPointLight();

	virtual void ActivateLight( void );

	void SetShadowCamera( CPointShadowCamera *pPointShadowCamera );

	void SetConstant( float flConstant );
	void SetLinear( float flLinear );
	void SetQuadratic( float flQuadratic );

protected:
	virtual void CalculateMaxRadius( void );

private:
	float m_flConstant;
	float m_flLinear;
	float m_flQuadratic;

	float m_flMaxRadius;
};

#endif // POINTLIGHT_H