#ifndef ANIMATEDPROP_H
#define ANIMATEDPROP_H

#include "Global.h"
#include "BaseAnimated.h"

class CAnimatedProp : public CBaseAnimated
{
public:
	DECLARE_CLASS( CAnimatedProp, CBaseAnimated )

	DECLARE_LINKED_CLASS()

	CAnimatedProp();

	virtual bool Init( void );
};

#endif // ANIMATEDPROP_H