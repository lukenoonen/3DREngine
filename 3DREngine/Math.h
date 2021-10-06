#ifndef MATH_H
#define MATH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

static const glm::vec3 g_vecUp = glm::vec3( 0.0f, 0.0f, 1.0f );
static const glm::vec3 g_vecDown = glm::vec3( 0.0f, 0.0f, -1.0f );
static const glm::vec3 g_vecFront = glm::vec3( 0.0f, 1.0f, 0.0f );
static const glm::vec3 g_vecBack = glm::vec3( 0.0f, -1.0f, 0.0f );
static const glm::vec3 g_vecRight = glm::vec3( 1.0f, 0.0f, 0.0f );
static const glm::vec3 g_vecLeft = glm::vec3( -1.0f, 0.0f, 0.0f );
static const glm::vec3 g_vecOne = glm::vec3( 1.0f, 1.0f, 1.0f );
static const glm::vec3 g_vecZero = glm::vec3( 0.0f, 0.0f, 0.0f );

static const glm::vec2 g_vec2Up = glm::vec2( 0.0f, 1.0f );
static const glm::vec2 g_vec2Down = glm::vec2( 0.0f, -1.0f );
static const glm::vec2 g_vec2Left = glm::vec2( -1.0f, 0.0f );
static const glm::vec2 g_vec2Right = glm::vec2( 1.0f, 0.0f );
static const glm::vec2 g_vec2One = glm::vec2( 1.0f, 1.0f );
static const glm::vec2 g_vec2Zero = glm::vec2( 0.0f, 0.0f );

static const glm::vec4 g_vec4Zero = glm::vec4( 0.0f );
static const glm::vec4 g_vec4One = glm::vec4( 1.0f );

#define M_SQRT2		1.41421356237f
#define M_PI		3.14159265359f

#endif // MATH_H