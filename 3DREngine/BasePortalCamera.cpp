#include "BasePortalCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

// TODO: FINISH REFACTORING THIS, REMOVE GetView and GetProjection OR WHATEVER

CBasePortalCamera::CBasePortalCamera()
{

}

bool CBasePortalCamera::Init( void )
{
	if (!BaseClass::Init())
		return false;

	UpdateTransform();

	return true;
}

void CBasePortalCamera::Think( void )
{
	if (ShouldUpdateTransform()) UpdateTransform();

	BaseClass::Think();
}

const glm::vec3 &CBasePortalCamera::GetCameraPosition( void ) const
{
	return m_vec3CameraPosition;
}

const glm::quat &CBasePortalCamera::GetCameraRotation( void ) const
{
	return m_qCameraRotation;
}

void CBasePortalCamera::PerformRender( void )
{
	pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_clip, (EBaseEnum)EShaderPreprocessorClip::t_true );
	glEnable( GL_CLIP_DISTANCE0 );

	pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_reflection, (EBaseEnum)EShaderPreprocessorReflection::t_false );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 0, &m_matTotal );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 1, &m_matTotalLocked );
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_view, 2, &GetPosition() );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_clip, 0, &m_vec4Plane );

	unsigned int bFlipPortal = ShouldFlipPortal();
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_reflection, 0, &bFlipPortal );

	pEntityManager->DrawEntities();

	pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_clip, (EBaseEnum)EShaderPreprocessorClip::t_false );
	glDisable( GL_CLIP_DISTANCE0 );

	pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_reflection, (EBaseEnum)EShaderPreprocessorReflection::t_true );
}

bool CBasePortalCamera::ShouldFlipPortal( void )
{
	return false;
}

bool CBasePortalCamera::ShouldUpdateView( void ) const
{
	CBaseWorldCamera *pTarget = GetTargetCamera();
	return BaseClass::ShouldUpdateView() || pTarget->PositionUpdated() || pTarget->RotationUpdated();
}

void CBasePortalCamera::UpdateView( void )
{
	BaseClass::UpdateView();

	CBaseWorldCamera *pTarget = GetTargetCamera();

	const glm::vec3 &vec3TargetPosition = pTarget->GetPosition();
	const glm::quat &qTargetRotation = pTarget->GetRotation();

	m_vec3CameraPosition = glm::vec4( vec3TargetPosition, 1.0f ) * m_matTransform;
	m_qCameraRotation = glm::quat_cast( m_matTransform ) * qTargetRotation;
}

bool CBasePortalCamera::ShouldUpdateProjection( void ) const
{
	CBaseWorldCamera *pTarget = GetTargetCamera();
	return pTarget->ShouldUpdateProjection();
}

glm::mat4 CBasePortalCamera::CalculateView( void ) const
{
	CBaseWorldCamera *pTarget = GetTargetCamera();

	const glm::vec3 &vec3TargetPosition = pTarget->GetPosition();
	const glm::quat &qTargetRotation = pTarget->GetRotation();

	glm::vec3 vec3Position = glm::vec3( m_matTransform * glm::vec4( vec3TargetPosition, 1.0f ) );
	glm::vec3 vec3Target = glm::vec3( m_matTransform * glm::vec4( vec3TargetPosition + qTargetRotation * g_vec3Front, 1.0f ) );
	
	return glm::lookAt( vec3Position, vec3Target, g_vec3Up );
}

glm::mat4 CBasePortalCamera::CalculateProjection( void ) const
{
	CBaseWorldCamera *pTarget = GetTargetCamera();
	return pTarget->CalculateProjection();
}

bool CBasePortalCamera::ShouldUpdateTransform( void ) const
{
	return PositionUpdated() || RotationUpdated();
}

void CBasePortalCamera::UpdateTransform( void )
{
	glm::vec3 vec3Normal = GetRotation() * g_vec3Front;
	m_vec4Plane = glm::vec4( vec3Normal, -glm::dot( vec3Normal, GetPosition() ) );
	m_matTransform = CalculateTransform();
}