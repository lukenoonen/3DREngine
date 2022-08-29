#ifndef BASEGLOBAL_H
#define BASEGLOBAL_H

#include <vector>
#include <unordered_map>

#include "Maths.h"
#include "UTIL.h"

#define DECLARE_CLASS( x, y )			typedef x ThisClass; \
										typedef y BaseClass;

#define DECLARE_CLASS_NOBASE( x )		typedef x ThisClass;

typedef unsigned char EBaseEnum;

#endif // BASEGLOBAL_H