#ifndef BASEWORLD_H
#define BASEWORLD_H

#include "Global.h"
#include "BaseTransform.h"

class CBaseWorld : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseWorld, CBaseTransform )

	CBaseWorld();

	virtual bool Init( void );

	virtual void PostThink( void );

	const glm::mat4 &GetModelMatrix( void ) const;
	const glm::mat4 &GetModelMatrixInverse( void ) const;

private:
	glm::mat4 m_matModel;
	glm::mat4 m_matModelInverse;
};

#endif // BASEWORLD_H