#ifndef BASEWORLD_H
#define BASEWORLD_H

#include "Global.h"
#include "BaseTransform.h"

class CBaseWorld : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseWorld, CBaseTransform );

	CBaseWorld( const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );

	virtual void PostThink( void );

	const glm::mat4 &GetModelMatrix( void ) const;
	const glm::mat4 &GetModelMatrixInverse( void ) const;

private:
	glm::mat4 m_matModel;
	glm::mat4 m_matModelInverse;
};

#endif // BASEWORLD_H