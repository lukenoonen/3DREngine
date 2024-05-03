#include "BaseBillboard.h"
#include "EntityManager.h"
#include "BaseCamera.h"
#include "RenderManager.h"

DEFINE_DATADESC( CBaseBillboard )

	DEFINE_FIELD( DataField, glm::vec3, m_vec3BillboardFactors, "billboardfactors", FL_NONE )

END_DATADESC()

CBaseBillboard::CBaseBillboard()
{
	m_vec3BillboardFactors = g_vec3One;
}

void CBaseBillboard::Draw( void )
{
	CBaseCamera *pCamera = pEntityManager->GetCurrentCamera();
	const glm::mat4 &matModelMatrix = GetModelMatrix();
	glm::mat4 billboardMatrix = glm::toMat4( glm::quat( glm::eulerAngles( pCamera->GetCameraRotation() * glm::inverse( GetRotation() ) ) * glm::vec3( m_vec3BillboardFactors ) ) );
	glm::mat4 adjustedMatrix = glm::translate( g_matIdentity, GetPosition() ) * billboardMatrix * glm::translate( g_matIdentity, -GetPosition() ) * matModelMatrix;
	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 0, &adjustedMatrix ); // TODO: change "model" matrix to better name
}