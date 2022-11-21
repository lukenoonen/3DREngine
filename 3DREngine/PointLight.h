#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Global.h"
#include "BaseLight.h"

class CPointLight : public CBaseLight
{
public:
	DECLARE_CLASS( CPointLight, CBaseLight )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CPointLight();

	virtual bool Init( void );

	virtual void ActivateLight( void );

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