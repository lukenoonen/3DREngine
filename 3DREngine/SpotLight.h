#ifndef SPOTLIGHT_H
#define SPOTLIGHT_H

#include "Global.h"
#include "BaseLight.h"
#include "SpotShadowCamera.h"

class CSpotLight : public CBaseLight
{
public:
	DECLARE_CLASS( CSpotLight, CBaseLight )

	CSpotLight();

	virtual void ActivateLight( void );

	void SetShadowCamera( CSpotShadowCamera *pSpotShadowCamera );

	void SetCutoff( float flCutoff );
	void SetOuterCutoff( float flOuterCutoff );

	void SetConstant( float flConstant );
	void SetLinear( float flLinear );
	void SetQuadratic( float flQuadratic );

protected:
	virtual void CalculateMaxRadius( void );

private:
	CSpotShadowCamera *m_pSpotShadowCamera;

	float m_flCutoff;
	float m_flOuterCutoff;

	float m_flConstant;
	float m_flLinear;
	float m_flQuadratic;

	float m_flMaxRadius;
};

#endif // SPOTLIGHT_H