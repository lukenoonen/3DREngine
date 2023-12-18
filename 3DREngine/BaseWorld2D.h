#ifndef BASEWORLD2D_H
#define BASEWORLD2D_H

#include "Global.h"
#include "BaseTransform.h"

enum class E2DAlign : EBaseEnum
{
	t_topleft = 0,
	t_topcenter,
	t_topright,
	t_middleleft,
	t_middlecenter,
	t_middleright,
	t_bottomleft,
	t_bottomcenter,
	t_bottomright,

	i_count,
	i_invalid = i_count,
};

static const char *g_s2DAlignNames[] =
{
	"topleft",
	"topcenter",
	"topright",
	"middleleft",
	"middlecenter",
	"middleright",
	"bottomleft",
	"bottomcenter",
	"bottomright",
};

DEFINE_ENUM_NAMES( E2DAlign, g_s2DAlignNames )

DEFINE_ENUM_GETVALUE( E2DAlign )

class CBaseWorld2D : public CBaseTransform
{
public:
	DECLARE_CLASS( CBaseWorld2D, CBaseTransform )

	DECLARE_DATADESC()

	CBaseWorld2D();

	virtual bool Init( void );

	virtual void PostThink( void );

	const glm::mat4 &GetModelMatrix( void ) const;

private:
	void UpdateModelMatrix( void );

	virtual const glm::vec2 &GetAlignFactor( void ) const;

private:
	E2DAlign m_e2DAlign;

	glm::mat4 m_matModel;
};

#endif // BASEWORLD3D_H