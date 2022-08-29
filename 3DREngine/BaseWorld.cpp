#include "BaseWorld.h"

CBaseWorld::CBaseWorld()
{

}

bool CBaseWorld::Init( void )
{
	if (!BaseClass::Init())
		return false;

	m_matModel = glm::translate( g_matIdentity, GetPosition() ) * glm::toMat4( GetRotation() ) * glm::scale( g_matIdentity, GetScale() );
	m_matModelInverse = glm::transpose( glm::inverse( m_matModel ) );

	return true;
}

void CBaseWorld::PostThink( void )
{
	if (PositionUpdated() || RotationUpdated() || ScaleUpdated())
	{
		m_matModel = glm::translate( g_matIdentity, GetPosition() ) * glm::toMat4( GetRotation() ) * glm::scale( g_matIdentity, GetScale() );
		m_matModelInverse = glm::transpose( glm::inverse( m_matModel ) );
	}

	BaseClass::PostThink();
}

const glm::mat4 &CBaseWorld::GetModelMatrix( void ) const
{
	return m_matModel;
}

const glm::mat4 &CBaseWorld::GetModelMatrixInverse( void ) const
{
	return m_matModelInverse;
}