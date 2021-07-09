#include "BaseWorld.h"

CBaseWorld::CBaseWorld( const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive ) : BaseClass( vecPosition, vecRotation, vecScale, bShouldDraw, bActive )
{

}

void CBaseWorld::PostThink( void )
{
	if (HasTransformUpdated())
	{
		m_matModel = glm::translate( glm::mat4( 1.0f ), GetPosition() ) * glm::toMat4( GetRotation() ) * glm::scale( glm::mat4( 1.0f ), GetScale() );
		m_matModelInverse = glm::transpose( glm::inverse( m_matModel ) );
		ResetTransformUpdated();
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