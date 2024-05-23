#ifndef BASEWORLD2D_H
#define BASEWORLD2D_H

#include "Global.h"
#include "BaseTransform.h"
#include "2DAlign.h"

DEFINE_ENTITY_FLAG( fl_absolute, 4 )

class CBaseWorld2D : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseWorld2D, CBaseTransform )

	DECLARE_DATADESC()

	CBaseWorld2D();

	virtual bool Init( void );

	virtual void PreRender( void );

	const glm::mat4 &GetModelMatrix( void ) const;

protected:
	bool ModelMatrixUpdated( void ) const;

private:
	void UpdateModelMatrix( void );

	virtual const glm::vec2 &GetAlignFactor( void ) const;
	virtual const glm::vec2 &GetOffset( void ) const;

private:
	E2DAlign m_e2DAlign;

	CMonitoredValue<glm::mat4> m_matModel;
};

#endif // BASEWORLD3D_H