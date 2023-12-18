#ifndef BASEWORLD3D_H
#define BASEWORLD3D_H

#include "Global.h"
#include "BaseTransform.h"

class CBaseWorld3D : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseWorld3D, CBaseTransform )

	CBaseWorld3D();

	virtual bool Init( void );

	virtual void PostThink( void );

	const glm::mat4 &GetModelMatrix( void ) const;
	const glm::mat4 &GetModelMatrixInverse( void ) const;

private:
	void UpdateModelMatrix( void );

private:
	glm::mat4 m_matModel;
	glm::mat4 m_matModelInverse;
};

#endif // BASEWORLD3D_H