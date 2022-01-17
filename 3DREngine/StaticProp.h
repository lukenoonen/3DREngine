#ifndef STATICPROP_H
#define STATICPROP_H

#include "Global.h"
#include "BaseModeled.h"

class CStaticProp : public CBaseModeled
{
public:
	DECLARE_CLASS( CStaticProp, CBaseModeled );

	CStaticProp( const char *sModelPath );
	virtual ~CStaticProp();
};

#endif // STATICPROP_H