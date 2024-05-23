#include "CameraAnchor.h"

DEFINE_DATADESC( CCameraAnchor )

	DEFINE_FIELD( LinkedDataField, CHandle<CBaseCamera>, m_hCamera, "camera", FL_REQUIRED )
	DEFINE_FIELD( DataField, E2DAlign, m_e2DAlign, "2dalign", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flDepth, "depth", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CCameraAnchor, camera_anchor )

CCameraAnchor::CCameraAnchor()
{
	m_e2DAlign = E2DAlign::t_middlecenter;
	m_flDepth = 1.0f;
}

bool CCameraAnchor::Init( void )
{
	m_qInverseCamera = glm::inverse( m_hCamera->GetRotation() );
	Anchor();

	if (!BaseClass::Init())
		return false;

	return true;
}

void CCameraAnchor::PostThink( void )
{
	// TODO: add depth and align change conditions, optimise! (make middleman class or something)
	if (m_hCamera->ShouldUpdateView() || m_hCamera->ShouldUpdateProjection())
		Anchor();

	BaseClass::PostThink();
}

void CCameraAnchor::Anchor( void )
{
	glm::vec2 vec2Align = UTIL_Get2DAlignOffset_ClipSpace( m_e2DAlign );
	glm::vec4 vecPosition = m_hCamera->GetProjection() * glm::vec4( 0.0f, 0.0f, -m_flDepth, 1.0f );
	vecPosition = glm::inverse( m_hCamera->GetTotal() ) * glm::vec4( vec2Align, vecPosition.z / vecPosition.w, 1.0f );
	vecPosition /= vecPosition.w;
	SetPosition( vecPosition );
	SetRotation( m_qInverseCamera * m_hCamera->GetRotation() );
}