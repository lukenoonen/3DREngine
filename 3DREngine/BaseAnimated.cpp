#include "BaseAnimated.h"
#include "AnimationManager.h"
#include "RenderManager.h"

CBaseAnimated::CBaseAnimated( const char *sAnimationControllerPath, const char *sMaterialPath, const char *sModelPath, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( sMaterialPath, sModelPath, vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{
	m_pAnimationController = pAnimationManager->GetAnimationController( sAnimationControllerPath );
	m_pAnimationController->SetBoneTransformSize( GetModel()->GetBoneTransformSize() );
}

void CBaseAnimated::PostThink( void )
{
	m_pAnimationController->UpdateAnimation();
	GetModel()->UpdateAnimation( m_pAnimationController );

	BaseClass::PostThink();
}

void CBaseAnimated::PreDraw( void )
{
	BaseClass::PreDraw();

	if (m_pAnimationController->ShouldAnimate())
	{
		const std::vector<glm::mat4> &matTransforms = m_pAnimationController->GetBoneTransforms();
		pShaderManager->SetUniformBufferObject( UBO_BONES, 0, 0, (unsigned int)UTIL_Min( (unsigned int)matTransforms.size(), 64 ), &matTransforms[0] );
	}
}