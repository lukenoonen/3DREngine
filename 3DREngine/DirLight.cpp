#include "DirLight.h"
#include "ShaderManager.h"

CDirLight::CDirLight( const glm::vec3 &vecAmbient, const glm::vec3 &vecDiffuse, const glm::vec3 &vecSpecular, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecAmbient, vecDiffuse, vecSpecular, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{

}

void CDirLight::ActivateLight( void )
{
	BaseClass::ActivateLight();

	pShaderManager->SetShaderSubType( SHADERSUBTYPE_DIR );

	pShaderManager->SetUniformBufferObject( UBO_LIGHTDIRECTION, 0, &(GetRotation() * g_vecFront) );
}