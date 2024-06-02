#ifndef BASEBILLBOARD_H
#define BASEBILLBOARD_H

#include "Global.h"
#include "BaseWorld2D.h"

class CBaseBillboard : public CBaseWorld2D
{
public:
	DECLARE_CLASS( CBaseBillboard, CBaseWorld2D )

	DECLARE_DATADESC()

	CBaseBillboard();

	virtual void PreRender( void );

	virtual void Draw( void );

private:
	glm::vec3 m_vec3BillboardFactors;
	glm::mat4 m_matAdjustedModel;
};

#endif // BASEBILLBOARD_H