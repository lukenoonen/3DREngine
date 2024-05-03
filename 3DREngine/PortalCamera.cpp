#include "PortalCamera.h"
#include "RenderManager.h"
#include "EntityManager.h"

DEFINE_DATADESC( CPortalCamera )

	DEFINE_FIELD( LinkedDataField, CHandle<CBaseTransform>, m_hDisplay, "display", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CPortalCamera, camera_portal )

CPortalCamera::CPortalCamera()
{

}

bool CPortalCamera::ShouldUpdateTransform( void ) const
{
	return BaseClass::ShouldUpdateTransform() || m_hDisplay->PositionUpdated() || m_hDisplay->RotationUpdated();
}

glm::mat4 CPortalCamera::CalculateTransform( void ) const
{
	glm::mat4 matRotation = glm::toMat4( glm::inverse( m_hDisplay->GetRotation() ) * GetRotation() );
	return glm::translate( g_matIdentity, GetPosition() - glm::vec3( matRotation * glm::vec4( m_hDisplay->GetPosition(), 1.0f ) ) ) * matRotation;
}