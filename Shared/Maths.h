#ifndef MATHS_H
#define MATHS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <TextReader/TextReader.h>

static const glm::vec3 g_vec3Up = glm::vec3( 0.0f, 0.0f, 1.0f );
static const glm::vec3 g_vec3Down = glm::vec3( 0.0f, 0.0f, -1.0f );
static const glm::vec3 g_vec3Front = glm::vec3( 0.0f, 1.0f, 0.0f );
static const glm::vec3 g_vec3Back = glm::vec3( 0.0f, -1.0f, 0.0f );
static const glm::vec3 g_vec3Right = glm::vec3( 1.0f, 0.0f, 0.0f );
static const glm::vec3 g_vec3Left = glm::vec3( -1.0f, 0.0f, 0.0f );
static const glm::vec3 g_vec3One = glm::vec3( 1.0f, 1.0f, 1.0f );
static const glm::vec3 g_vec3Zero = glm::vec3( 0.0f, 0.0f, 0.0f );

static const glm::vec2 g_vec2Up = glm::vec2( 0.0f, 1.0f );
static const glm::vec2 g_vec2Down = glm::vec2( 0.0f, -1.0f );
static const glm::vec2 g_vec2Left = glm::vec2( -1.0f, 0.0f );
static const glm::vec2 g_vec2Right = glm::vec2( 1.0f, 0.0f );
static const glm::vec2 g_vec2One = glm::vec2( 1.0f, 1.0f );
static const glm::vec2 g_vec2Zero = glm::vec2( 0.0f, 0.0f );
static const glm::vec2 g_vec2FlipHorizontal = glm::vec2( -1.0f, 1.0f );
static const glm::vec2 g_vec2FlipVertical = glm::vec2( 1.0f, -1.0f );

static const glm::vec4 g_vec4Zero = glm::vec4( 0.0f );
static const glm::vec4 g_vec4One = glm::vec4( 1.0f );

static const glm::ivec3 g_ivec3Up = glm::ivec3( 0, 0, 1 );
static const glm::ivec3 g_ivec3Down = glm::ivec3( 0, 0, -1 );
static const glm::ivec3 g_ivec3Front = glm::ivec3( 0, 1, 0 );
static const glm::ivec3 g_ivec3Back = glm::ivec3( 0, -1, 0 );
static const glm::ivec3 g_ivec3Right = glm::ivec3( 1, 0, 0 );
static const glm::ivec3 g_ivec3Left = glm::ivec3( -1, 0, 0 );
static const glm::ivec3 g_ivec3One = glm::ivec3( 1, 1, 1 );
static const glm::ivec3 g_ivec3Zero = glm::ivec3( 0, 0, 0 );

static const glm::ivec2 g_ivec2Up = glm::ivec2( 0, 1 );
static const glm::ivec2 g_ivec2Down = glm::ivec2( 0, -1 );
static const glm::ivec2 g_ivec2Left = glm::ivec2( -1, 0 );
static const glm::ivec2 g_ivec2Right = glm::ivec2( 1, 0 );
static const glm::ivec2 g_ivec2One = glm::ivec2( 1, 1 );
static const glm::ivec2 g_ivec2Zero = glm::ivec2( 0, 0 );

static const glm::ivec4 g_ivec4Zero = glm::ivec4( 0 );
static const glm::ivec4 g_ivec4One = glm::ivec4( 1 );

static const glm::mat4 g_matIdentity = glm::mat4( 1.0f );
static const glm::mat4 g_matZero = glm::mat4( 0.0f );

static const glm::quat g_qZero = glm::quat( glm::vec3( 0.0f, 0.0f, 0.0f ) );

#define M_SQRT2		1.41421356237f
#define M_PI		3.14159265359f

template <class T> inline T UTIL_max( T a, T b )
{
	return a > b ? a : b;
}

template <class T> inline T UTIL_min( T a, T b )
{
	return a < b ? a : b;
}

glm::quat UTIL_RotationDifference( glm::vec3 vecStart, glm::vec3 vecEnd );

bool UTIL_GetValue( const CTextItem *pTextItem, glm::vec2 &vecValue );
bool UTIL_GetValue( const CTextItem *pTextItem, glm::vec3 &vecValue );
bool UTIL_GetValue( const CTextItem *pTextItem, glm::vec4 &vecValue );
bool UTIL_GetValue( const CTextItem *pTextItem, glm::ivec2 &vecValue );
bool UTIL_GetValue( const CTextItem *pTextItem, glm::ivec3 &vecValue );
bool UTIL_GetValue( const CTextItem *pTextItem, glm::ivec4 &vecValue );
bool UTIL_GetValue( const CTextItem *pTextItem, glm::quat &qValue );

#endif // MATH_H