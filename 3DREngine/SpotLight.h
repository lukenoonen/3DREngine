#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Global.h"
#include "BaseLight.h"

class CSpotLight : public CBaseLight
{
public:
	DECLARE_CLASS( CSpotLight, CBaseLight )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CSpotLight();

	virtual bool Init( void );

	virtual void ActivateLight( void );

	void SetCutoff( float flCutoff );
	void SetOuterCutoff( float flOuterCutoff );

	void SetConstant( float flConstant );
	void SetLinear( float flLinear );
	void SetQuadratic( float flQuadratic );

protected:
	virtual void CalculateMaxRadius( void );


private:
	float m_flCutoff;
	float m_flOuterCutoff;

	float m_flConstant;
	float m_flLinear;
	float m_flQuadratic;

	float m_flMaxRadius;
};

#endif // SPOTLIGHT_H