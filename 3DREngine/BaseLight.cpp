#include "BaseLight.h"
#include "ShaderManager.h"

CBaseLight::CBaseLight()
{
	m_pShadowCamera = NULL;

	m_vecAmbient = g_vecOne * 0.1f;
	m_vecDiffuse = g_vecOne * 0.9f;
	m_vecSpecular = g_vecOne * 0.4f;
}

bool CBaseLight::IsLight( void ) const
{
	return true;
}

void CBaseLight::ActivateLight( void )
{
	pShaderManager->SetUniformBufferObject( UBO_LIGHT, 0, &m_vecAmbient );
	pShaderManager->SetUniformBufferObject( UBO_LIGHT, 1, &m_vecDiffuse );
	pShaderManager->SetUniformBufferObject( UBO_LIGHT, 2, &m_vecSpecular );

	if (m_pShadowCamera)
	{
		pShaderManager->SetShaderShadow( SHADERSHADOW_TRUE );
		m_pShadowCamera->ActivateLight();
	}
	else
	{
		pShaderManager->SetShaderShadow( SHADERSHADOW_FALSE );
	}
}

void CBaseLight::SetAmbient( const glm::vec3 &vecAmbient )
{
	m_vecAmbient = vecAmbient;
}

void CBaseLight::SetDiffuse( const glm::vec3 &vecDiffuse )
{
	m_vecDiffuse = vecDiffuse;
}

void CBaseLight::SetSpecular( const glm::vec3 &vecSpecular )
{
	m_vecSpecular = vecSpecular;
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
	return std::fmaxf( std::fmaxf( m_vecDiffuse.r, m_vecDiffuse.g ), m_vecDiffuse.b );
}