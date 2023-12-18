#include "BaseLight.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_DATADESC( CBaseLight )

	DEFINE_FIELD( DataField, glm::vec3, m_vec3Ambient, "ambient", 0 )
	DEFINE_FIELD( DataField, glm::vec3, m_vec3Diffuse, "diffuse", 0 )
	DEFINE_FIELD( DataField, glm::vec3, m_vec3Specular, "specular", 0 )
	DEFINE_FIELD( LinkedDataField, CHandle<CBaseShadowCamera>, m_hShadowCamera, "shadowcamera", 0 )

END_DATADESC()

CBaseLight::CBaseLight()
{
	m_vec3Ambient = g_vec3One * 0.1f;
	m_vec3Diffuse = g_vec3One * 0.9f;
	m_vec3Specular = g_vec3One * 0.4f;
}

bool CBaseLight::IsLight( void ) const
{
	return true;
}

void CBaseLight::PostThink( void )
{
	m_hShadowCamera.Check();

	BaseClass::PostThink();
}

void CBaseLight::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_light, 0, &m_vec3Ambient );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_light, 1, &m_vec3Diffuse );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_light, 2, &m_vec3Specular );

	if (m_hShadowCamera)
		m_hShadowCamera->ActivateLight();
}

void CBaseLight::SetAmbient( const glm::vec3 &vec3Ambient )
{
	m_vec3Ambient = vec3Ambient;
}

void CBaseLight::SetDiffuse( const glm::vec3 &vec3Diffuse )
{
	m_vec3Diffuse = vec3Diffuse;
	CalculateMaxRadius();
}

void CBaseLight::SetSpecular( const glm::vec3 &vec3Specular )
{
	m_vec3Specular = vec3Specular;
}

CBaseShadowCamera *CBaseLight::GetShadowCamera( void ) const
{
	return m_hShadowCamera;
}

void CBaseLight::CalculateMaxRadius( void )
{

}

float CBaseLight::GetMaxDiffuse( void ) const
{
	return std::fmaxf( std::fmaxf( m_vec3Diffuse.r, m_vec3Diffuse.g ), m_vec3Diffuse.b );
}