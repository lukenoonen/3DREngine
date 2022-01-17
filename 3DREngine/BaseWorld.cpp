#include "BaseWorld.h"

CBaseWorld::CBaseWorld()
{
	m_matModel = glm::translate( glm::mat4( 1.0f ), GetPosition() ) * glm::toMat4( GetRotation() ) * glm::scale( glm::mat4( 1.0f ), GetScale() );
	m_matModelInverse = glm::transpose( glm::inverse( m_matModel ) );
}

void CBaseWorld::PostThink( void )
{
	if (PositionUpdated() || RotationUpdated() || ScaleUpdated())
	{
		m_matModel = glm::translate( glm::mat4( 1.0f ), GetPosition() ) * glm::toMat4( GetRotation() ) * glm::scale( glm::mat4( 1.0f ), GetScale() );
		m_matModelInverse = glm::transpose( glm::inverse( m_matModel ) );
	}
}

const glm::mat4 &CBaseWorld::GetModelMatrix( void ) const
{
	return m_matModel;
}

const glm::mat4 &CBaseWorld::GetModelMatrixInverse( void ) const
{
	return m_matModelInverse;
}