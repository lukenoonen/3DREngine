#include "BaseWorld2D.h"

DEFINE_DATADESC( CBaseWorld2D )

	DEFINE_FIELD( DataField, E2DAlign, m_e2DAlign, "2dalign", 0 )

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

void CBaseWorld2D::PostThink( void )
{
	if (PositionUpdated() || RotationUpdated() || ScaleUpdated())
		UpdateModelMatrix();

	UpdateModelMatrix();

	BaseClass::PostThink();
}

const glm::mat4 &CBaseWorld2D::GetModelMatrix( void ) const
{
	return m_matModel;
}

void CBaseWorld2D::UpdateModelMatrix( void )
{
	glm::vec2 vec2Align = g_vec2Zero;

	switch (m_e2DAlign)
	{
	case E2DAlign::t_topleft:
	{
		vec2Align = glm::vec2( 0.0f, 0.0f );
		break;
	}
	case E2DAlign::t_topcenter:
	{
		vec2Align = glm::vec2( 0.5f, 0.0f );
		break;
	}
	case E2DAlign::t_topright:
	{
		vec2Align = glm::vec2( 1.0f, 0.0f );
		break;
	}
	case E2DAlign::t_middleleft:
	{
		vec2Align = glm::vec2( 0.0f, 0.5f );
		break;
	}
	case E2DAlign::t_middlecenter:
	{
		vec2Align = glm::vec2( 0.5f, 0.5f );
		break;
	}
	case E2DAlign::t_middleright:
	{
		vec2Align = glm::vec2( 1.0f, 0.5f );
		break;
	}
	case E2DAlign::t_bottomleft:
	{
		vec2Align = glm::vec2( 0.0f, 1.0f );
		break;
	}
	case E2DAlign::t_bottomcenter:
	{
		vec2Align = glm::vec2( 0.5f, 1.0f );
		break;
	}
	case E2DAlign::t_bottomright:
	{
		vec2Align = glm::vec2( 1.0f, 1.0f );
		break;
	}
	}

	m_matModel =
		glm::translate( g_matIdentity, GetPosition() ) *
		glm::toMat4( GetRotation() ) *
		glm::translate( g_matIdentity, glm::vec3( glm::vec2( -GetScale() ) * GetAlignFactor() * vec2Align * g_vec2FlipVertical, 0.0f)) *
		glm::scale( g_matIdentity, GetScale() );
}

const glm::vec2 &CBaseWorld2D::GetAlignFactor( void ) const
{
	return g_vec2One;
}