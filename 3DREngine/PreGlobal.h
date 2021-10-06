#ifndef PREGLOBAL_H
#define PREGLOBAL_H

#include <Windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <limits>
#include "UTIL.h"
#include "Math.h"

#define DECLARE_CLASS( x, y )			typedef x ThisClass; \
										typedef y BaseClass;

#define DECLARE_CLASS_NOBASE( x )		typedef x ThisClass;

#endif // PREGLOBAL_H