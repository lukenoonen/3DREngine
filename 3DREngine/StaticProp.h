#ifndef STATICPROP_H
#define STATICPROP_H

#include "Global.h"
#include "BaseModeled.h"

class CStaticProp : public CBaseModeled
{
public:
	DECLARE_CLASS( CStaticProp, CBaseModeled )

	DECLARE_LINKED_CLASS()

	CStaticProp();
};

#endif // STATICPROP_H