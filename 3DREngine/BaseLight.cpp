#include "BaseLight.h"
#include "ShaderManager.h"

CBaseLight::CBaseLight( CBaseShadowCamera *pShadowCamera, const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular )
{
	m_pShadowCamera = pShadowCamera;
	if (m_pShadowCamera)
	{
		m_pShadowCamera->SetPosition( GetPosition() );
		m_pShadowCamera->SetRotation( GetRotation() );
		m_pShadowCamera->SetScale( GetScale() );
		m_pShadowCamera->SetParent( this );
	}

	m_vecAmbient = vecAmbient;
	m_vecDiffuse = vecDiffuse;
	m_vecSpecular = vecSpecular;
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