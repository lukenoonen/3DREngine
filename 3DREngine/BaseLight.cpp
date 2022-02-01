#include "BaseLight.h"
#include "RenderManager.h"
#include "EntityManager.h"

CBaseLight::CBaseLight()
{
	m_pShadowCamera = NULL;

	m_vec3Ambient = g_vec3One * 0.1f;
	m_vec3Diffuse = g_vec3One * 0.9f;
	m_vec3Specular = g_vec3One * 0.4f;
}

bool CBaseLight::IsLight( void ) const
{
	return true;
}

void CBaseLight::ActivateLight( void )
{
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_light, 0, &m_vec3Ambient );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_light, 1, &m_vec3Diffuse );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_light, 2, &m_vec3Specular );

	if (m_pShadowCamera)
		m_pShadowCamera->ActivateLight();

	pEntityManager->SetShadowCamera( m_pShadowCamera );
}

void CBaseLight::SetAmbient( const glm::vec3 &vec3Ambient )
{
	m_vec3Ambient = vec3Ambient;
}

void CBaseLight::SetDiffuse( const glm::vec3 &vec3Diffuse )
{
	m_vec3Diffuse = vec3Diffuse;
}

void CBaseLight::SetSpecular( const glm::vec3 &vec3Specular )
{
	m_vec3Specular = vec3Specular;
}

void CBaseLight::SetShadowCamera( CBaseShadowCamera *pShadowCamera )
{
	if (m_pShadowCamera != pShadowCamera)
	{
		if (m_pShadowCamera)
		{
			m_pShadowCamera->SetParent( NULL );
		}

		m_pShadowCamera = pShadowCamera;

		if (m_pShadowCamera)
		{
			m_pShadowCamera->SetPosition( GetPosition() );
			m_pShadowCamera->SetRotation( GetRotation() );
			m_pShadowCamera->SetScale( GetScale() );
			m_pShadowCamera->SetParent( this );
		}
	}
}

void CBaseLight::CalculateMaxRadius( void )
{

}

float CBaseLight::GetMaxDiffuse( void ) const
{
	return std::fmaxf( std::fmaxf( m_vec3Diffuse.r, m_vec3Diffuse.g ), m_vec3Diffuse.b );
}