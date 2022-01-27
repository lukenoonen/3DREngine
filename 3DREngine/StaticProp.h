#ifndef STATICPROP_H
#define STATICPROP_H

#include "Global.h"
#include "BaseModeled.h"

class CStaticProp : public CBaseModeled
{
public:
	DECLARE_CLASS( CStaticProp, CBaseModeled );

	CStaticProp();

	virtual void Exit( void );

	void SetModel( const char *sModelPath );
};

#endif // STATICPROP_H