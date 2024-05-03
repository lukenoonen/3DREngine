#include "SpotLight.h"
#include "RenderManager.h"
#include "SpotShadowCamera.h"

DEFINE_DATADESC( CSpotLight )

	DEFINE_FIELD( DataField, float, m_flCutoff, "cutoff", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flOuterCutoff, "outercutoff", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flConstant, "constant", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flLinear, "linear", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flQuadratic, "quadratic", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CSpotLight, light_spot )

CSpotLight::CSpotLight()
{
	m_flCutoff = 0.7660f;
	m_flOuterCutoff = 0.7071f;

	m_flConstant = 1.0f;
	m_flLinear = 0.09f;
	m_flQuadratic = 0.032f;
}

bool CSpotLight::Init( void )
{
	if (!BaseClass::Init())
		return false;

	CBaseShadowCamera *pShadowCamera = GetShadowCamera();
	if (pShadowCamera)
	{
		CSpotShadowCamera *pSpotShadowCamera = dynamic_cast<CSpotShadowCamera *>(pShadowCamera);
		if (!pSpotShadowCamera)
			return false;

		pSpotShadowCamera->SetOuterCutoff( m_flOuterCutoff );
	}

	m_flCutoff = glm::cos( m_flCutoff );
	m_flOuterCutoff = glm::cos( m_flOuterCutoff );

	CalculateMaxRadius();

	return true;
}

void CSpotLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pRenderManager->SetRenderPass( ERenderPass::t_litspot );

	glm::vec3 vec3Direction = GetRotation() * g_vec3Front;
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightposition, 0, &GetPosition() );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 0, &m_flConstant );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 1, &m_flLinear );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightpoint, 2, &m_flQuadratic );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightmaxdistance, 0, &m_flMaxRadius );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightdirection, 0, &vec3Direction );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightspot, 0, &m_flCutoff );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_lightspot, 1, &m_flOuterCutoff );
}

void CSpotLight::SetCutoff( float flCutoff )
{
	m_flCutoff = glm::cos( flCutoff );
}

void CSpotLight::SetOuterCutoff( float flOuterCutoff )
{
	m_flOuterCutoff = glm::cos( flOuterCutoff );

	CSpotShadowCamera *pSpotShadowCamera = static_cast<CSpotShadowCamera *>(GetShadowCamera());
	if (pSpotShadowCamera)
		pSpotShadowCamera->SetOuterCutoff( flOuterCutoff );
}

void CSpotLight::SetConstant( float flConstant )
{
	m_flConstant = flConstant;
	CalculateMaxRadius();
}

void CSpotLight::SetLinear( float flLinear )
{
	m_flLinear = flLinear;
	CalculateMaxRadius();
}

void CSpotLight::SetQuadratic( float flQuadratic )
{
	m_flQuadratic = flQuadratic;
	CalculateMaxRadius();
}

void CSpotLight::CalculateMaxRadius( void )
{
	m_flMaxRadius = (-m_flConstant + std::sqrtf( m_flLinear * m_flLinear - 4.0f * m_flQuadratic * (m_flQuadratic - 100.0f * GetMaxDiffuse()) )) / (2.0f * m_flQuadratic);
}