#ifndef MATHS_H
#define MATHS_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

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

static const glm::vec4 g_vec4Zero = glm::vec4( 0.0f );
static const glm::vec4 g_vec4One = glm::vec4( 1.0f );

static const glm::mat4 g_matIdentity = glm::mat4( 1.0f );
static const glm::mat4 g_matZero = glm::mat4( 0.0f );

static const glm::quat g_qZero = glm::quat( glm::vec3( 0.0f, 0.0f, 0.0f ) );

#define M_SQRT2		1.41421356237f
#define M_PI		3.14159265359f

#endif // MATH_H