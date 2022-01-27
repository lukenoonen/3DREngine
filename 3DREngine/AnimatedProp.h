#ifndef ANIMATEDPROP_H
#define ANIMATEDPROP_H

#include "Global.h"
#include "BaseAnimated.h"

class CAnimatedProp : public CBaseAnimated
{
public:
	DECLARE_CLASS( CAnimatedProp, CBaseAnimated );

	CAnimatedProp();

	virtual void Init( void );
	virtual void Exit( void );

	void SetModel( const char *sModelPath );
};

#endif // ANIMATEDPROP_H