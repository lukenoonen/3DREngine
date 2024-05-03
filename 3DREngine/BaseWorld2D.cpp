#include "BaseWorld2D.h"

DEFINE_DATADESC( CBaseWorld2D )

	DEFINE_FIELD( DataField, E2DAlign, m_e2DAlign, "2dalign", FL_NONE )

END_DATADESC()

CBaseWorld2D::CBaseWorld2D()
{
	m_e2DAlign = E2DAlign::t_topleft;
}

bool CBaseWorld2D::Init( void )
{
	if (!BaseClass::Init())
		return false;

	UpdateModelMatrix();
	return true;
}

void CBaseWorld2D::PreRender( void )
{
	if (PositionUpdated() || RotationUpdated() || ScaleUpdated())
		UpdateModelMatrix();

	BaseClass::PreRender();
}

const glm::mat4 &CBaseWorld2D::GetModelMatrix( void ) const
{
	return m_matModel.Get();
}

bool CBaseWorld2D::ModelMatrixUpdated( void ) const
{
	return m_matModel.Modified();
}

void CBaseWorld2D::UpdateModelMatrix( void )
{
	glm::vec2 vec2Align = UTIL_Get2DAlignOffset_ScreenSpace( m_e2DAlign );
	m_matModel =
		glm::translate( g_matIdentity, GetPosition() ) *
		glm::toMat4( GetRotation() ) *
		glm::translate( g_matIdentity, glm::vec3( glm::vec2( -GetScale() ) * GetAlignFactor() * vec2Align * g_vec2FlipVertical, 0.0f ) ) *
		glm::scale( g_matIdentity, GetScale() );
}

const glm::vec2 &CBaseWorld2D::GetAlignFactor( void ) const
{
	return g_vec2One;
}