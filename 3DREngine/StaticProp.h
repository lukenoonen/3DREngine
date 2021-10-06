#ifndef STATICPROP_H
#define STATICPROP_H

#include "Global.h"
#include "BaseModeled.h"

class CStaticProp : public CBaseModeled
{
public:
	DECLARE_CLASS( CStaticProp, CBaseModeled );

	CStaticProp( const char *sModelPath, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CStaticProp();
};

#endif // STATICPROP_H