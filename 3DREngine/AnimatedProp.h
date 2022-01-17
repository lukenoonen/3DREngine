#ifndef ANIMATEDPROP_H
#define ANIMATEDPROP_H

#include "Global.h"
#include "BaseAnimated.h"

class CAnimatedProp : public CBaseAnimated
{
public:
	DECLARE_CLASS( CAnimatedProp, CBaseAnimated );

	CAnimatedProp( const char *sModelPath );
	virtual ~CAnimatedProp();
};

#endif // ANIMATEDPROP_H