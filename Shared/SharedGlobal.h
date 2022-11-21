#ifndef SHAREDGLOBAL_H
#define SHAREDGLOBAL_H

#include <vector>
#include <stack>
#include <unordered_map>
#include <TextReader/TextReader.h>
#include <FileReader/FileReader.h>

#include "Maths.h"
#include "Strings.h"

#define DECLARE_CLASS( x, y )			typedef x ThisClass; \
										typedef y BaseClass;

#define DECLARE_CLASS_NOBASE( x )		typedef x ThisClass;

typedef unsigned char EBaseEnum;

#endif // SHAREDGLOBAL_H