#include "BaseWorld3D.h"

CBaseWorld3D::CBaseWorld3D()
{

}

bool CBaseWorld3D::Init( void )
{
	if (!BaseClass::Init())
		return false;

	UpdateModelMatrix();
	return true;
}

void CBaseWorld3D::PostThink( void )
{
	if (PositionUpdated() || RotationUpdated() || ScaleUpdated())
		UpdateModelMatrix();

	BaseClass::PostThink();
}

const glm::mat4 &CBaseWorld3D::GetModelMatrix( void ) const
{
	return m_matModel;
}

const glm::mat4 &CBaseWorld3D::GetModelMatrixInverse( void ) const
{
	return m_matModelInverse;
}

void CBaseWorld3D::UpdateModelMatrix( void )
{
	m_matModel = glm::translate( g_matIdentity, GetPosition() ) * glm::toMat4( GetRotation() ) * glm::scale( g_matIdentity, GetScale() );
	m_matModelInverse = glm::transpose( glm::inverse( m_matModel ) );
}