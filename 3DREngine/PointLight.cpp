#include "PointLight.h"
#include "RenderManager.h"
#include "PointShadowCamera.h"

DEFINE_DATADESC( CPointLight )

	DEFINE_FIELD( DataField, float, m_flConstant, "constant", 0 )
	DEFINE_FIELD( DataField, float, m_flLinear, "linear", 0 )
	DEFINE_FIELD( DataField, float, m_flQuadratic, "quadratic", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CPointLight, light_point )

CPointLight::CPointLight()
{
	m_flConstant = 1.0f;
	m_flLinear = 0.09f;
	m_flQuadratic = 0.032f;
}

bool CPointLight::Init( void )
{
	if (!BaseClass::Init())
		return false;

	CBaseShadowCamera *pShadowCamera = GetShadowCamera();
	if (pShadowCamera && !(dynamic_cast<CPointShadowCamera *>(pShadowCamera)))
		return false;

	CalculateMaxRadius();

	if (pShadowCamera)
		((CPointShadowCamera *)pShadowCamera)->SetFar( m_flMaxRadius );

	return true;
}

void CPointLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( ERenderPass::t_litpoint );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightposition, 0, &GetPosition() );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 0, &m_flConstant );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 1, &m_flLinear );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 2, &m_flQuadratic );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightmaxdistance, 0, &m_flMaxRadius );
}

void CPointLight::SetConstant( float flConstant )
{
	m_flConstant = flConstant;
	CalculateMaxRadius();
}

void CPointLight::SetLinear( float flLinear )
{
	m_flLinear = flLinear;
	CalculateMaxRadius();
}

void CPointLight::SetQuadratic( float flQuadratic )
{
	m_flQuadratic = flQuadratic;
	CalculateMaxRadius();
}

void CPointLight::CalculateMaxRadius( void )
{
	m_flMaxRadius = (-m_flConstant + std::sqrtf( m_flLinear * m_flLinear - 4.0f * m_flQuadratic * (m_flQuadratic - 100.0f * GetMaxDiffuse()) )) / (2.0f * m_flQuadratic);
}