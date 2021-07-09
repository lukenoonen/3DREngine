#include "BaseLight.h"
#include "TextureManager.h"
#include "RenderManager.h"
#include "ShaderManager.h"

CBaseLight::CBaseLight( const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_vecAmbient = vecAmbient;
	m_vecDiffuse = vecDiffuse;
	m_vecSpecular = vecSpecular;
}

bool CBaseLight::IsLight( void ) const
{
	return true;
}

bool CBaseLight::CastShadows( void ) const
{
	return false;
}

void CBaseLight::CalculateShadows( void )
{

}

void CBaseLight::ActivateLight( void )
{
	int bCastShadows = (int)CastShadows();
	pShaderManager->SetUniformBufferObject( UBO_LIGHT, 0, &m_vecAmbient );
	pShaderManager->SetUniformBufferObject( UBO_LIGHT, 1, &m_vecDiffuse );
	pShaderManager->SetUniformBufferObject( UBO_LIGHT, 2, &m_vecSpecular );
	pShaderManager->SetUniformBufferObject( UBO_LIGHT, 3, &bCastShadows );
}