#ifndef ANIMATEDPROP_H
#define ANIMATEDPROP_H

#include "Global.h"
#include "BaseAnimated.h"

class CAnimatedProp : public CBaseAnimated
{
public:
	DECLARE_CLASS( CAnimatedProp, CBaseAnimated );

	CAnimatedProp( const char *sModelPath, const glm::vec3 &vecPosition, const glm::vec3 &vecRotation, const glm::vec3 &vecScale, bool bShouldDraw, bool bActive );
	virtual ~CAnimatedProp();
};

#endif // ANIMATEDPROP_H