#ifndef UNIFORMBUFFEROBJECTS_H
#define UNIFORMBUFFEROBJECTS_H

#include "Global.h"

// TODO: consider making macros to help clean this up and make it similar to how commands and entity factories work

struct SViewBuffer
{
	glm::mat4 matProjectionView;
	glm::mat4 matProjectionViewLocked;
	glm::vec3 vec3ViewPos;
};

static GLintptr g_glUBOViewParamOffsets[] =
{
	offsetof( SViewBuffer, matProjectionView ),
	offsetof( SViewBuffer, matProjectionViewLocked ),
	offsetof( SViewBuffer, vec3ViewPos ),
};

static GLsizeiptr g_glUBOViewParamSizes[] =
{
	sizeof( SViewBuffer().matProjectionView ),
	sizeof( SViewBuffer().matProjectionViewLocked ),
	sizeof( SViewBuffer().vec3ViewPos ),
};

struct SModelBuffer
{
	glm::mat4 matModel;
	glm::mat4 matTIModel;
};

static GLintptr g_glUBOModelParamOffsets[] =
{
	offsetof( SModelBuffer, matModel ),
	offsetof( SModelBuffer, matTIModel ),
};

static GLsizeiptr g_glUBOModelParamSizes[] =
{
	sizeof( SModelBuffer().matModel ),
	sizeof( SModelBuffer().matTIModel ),
};

struct SBonesBuffer
{
	glm::mat4 matBones[64];
};

static GLintptr g_glUBOBonesParamOffsets[] =
{
	offsetof( SBonesBuffer, matBones ),
};

static GLsizeiptr g_glUBOBonesParamSizes[] =
{
	sizeof( SBonesBuffer().matBones[0] ),
};

struct SClipBuffer
{
	glm::vec4 vec4ClipPlane;
};

static GLintptr g_glUBOClipParamOffsets[] =
{
	offsetof( SClipBuffer, vec4ClipPlane ),
};

static GLsizeiptr g_glUBOClipParamSizes[] =
{
	sizeof( SClipBuffer().vec4ClipPlane ),
};

struct SLightBuffer
{
	glm::vec3 vec3Ambient;
	float _BUFFER0_;
	glm::vec3 vec3Diffuse;
	float _BUFFER1_;
	glm::vec3 vec3Specular;
};

static GLintptr g_glUBOLightParamOffsets[] =
{
	offsetof( SLightBuffer, vec3Ambient ),
	offsetof( SLightBuffer, vec3Diffuse ),
	offsetof( SLightBuffer, vec3Specular ),
};

static GLsizeiptr g_glUBOLightParamSizes[] =
{
	sizeof( SLightBuffer().vec3Ambient ),
	sizeof( SLightBuffer().vec3Diffuse ),
	sizeof( SLightBuffer().vec3Specular ),
};

struct SLightPositionBuffer
{
	glm::vec3 vec3Position;
};

static GLintptr g_glUBOLightPositionParamOffsets[] =
{
	offsetof( SLightPositionBuffer, vec3Position ),
};

static GLsizeiptr g_glUBOLightPositionParamSizes[] =
{
	sizeof( SLightPositionBuffer().vec3Position ),
};

struct SLightDirectionBuffer
{
	glm::vec3 vec3Direction;
};

static GLintptr g_glUBOLightDirectionParamOffsets[] =
{
	offsetof( SLightDirectionBuffer, vec3Direction ),
};

static GLsizeiptr g_glUBOLightDirectionParamSizes[] =
{
	sizeof( SLightDirectionBuffer().vec3Direction ),
};

struct SLightPointBuffer
{
	float flConstant;
	float flLinear;
	float flQuadratic;
};

static GLintptr g_glUBOLightPointParamOffsets[] =
{
	offsetof( SLightPointBuffer, flConstant ),
	offsetof( SLightPointBuffer, flLinear ),
	offsetof( SLightPointBuffer, flQuadratic ),
};

static GLsizeiptr g_glUBOLightPointParamSizes[] =
{
	sizeof( SLightPointBuffer().flConstant ),
	sizeof( SLightPointBuffer().flLinear ),
	sizeof( SLightPointBuffer().flQuadratic ),
};

struct SLightSpotBuffer
{
	float flCutOff;
	float flOuterCutOff;
};

static GLintptr g_glUBOLightSpotParamOffsets[] =
{
	offsetof( SLightSpotBuffer, flCutOff ),
	offsetof( SLightSpotBuffer, flOuterCutOff ),
};

static GLsizeiptr g_glUBOLightSpotParamSizes[] =
{
	sizeof( SLightSpotBuffer().flCutOff ),
	sizeof( SLightSpotBuffer().flOuterCutOff ),
};

struct SLightMaxDistanceBuffer
{
	float flMaxDistance;
};

static GLintptr g_glUBOLightMaxDistanceParamOffsets[] =
{
	offsetof( SLightMaxDistanceBuffer, flMaxDistance ),
};

static GLsizeiptr g_glUBOLightMaxDistanceParamSizes[] =
{
	sizeof( SLightMaxDistanceBuffer().flMaxDistance ),
};

struct SShadowBuffer
{
	glm::mat4 matShadowMatrix[6];
};

static GLintptr g_glUBOShadowParamOffsets[] =
{
	offsetof( SShadowBuffer, matShadowMatrix ),
};

static GLsizeiptr g_glUBOShadowParamSizes[] =
{
	sizeof( SShadowBuffer().matShadowMatrix[0] ),
};

struct SShadowBlurBuffer
{
	float flBlurScale;
};

static GLintptr g_glUBOShadowBlurParamOffsets[] =
{
	offsetof( SShadowBlurBuffer, flBlurScale ),
};

static GLsizeiptr g_glUBOShadowBlurParamSizes[] =
{
	sizeof( SShadowBlurBuffer().flBlurScale ),
};

struct SShadowFadeBuffer
{
	float flShadowFadeNear;
	float flShadowFadeFar;
};

static GLintptr g_glUBOShadowFadeParamOffsets[] =
{
	offsetof( SShadowFadeBuffer, flShadowFadeNear ),
	offsetof( SShadowFadeBuffer, flShadowFadeFar ),
};

static GLsizeiptr g_glUBOShadowFadeParamSizes[] =
{
	sizeof( SShadowFadeBuffer().flShadowFadeNear ),
	sizeof( SShadowFadeBuffer().flShadowFadeFar ),
};

struct SShadowCascadeFadeBuffer
{
	glm::vec4 vec4ShadowCascadeFadeNear;
	glm::vec4 vec4ShadowCascadeFadeFar;
};

static GLintptr g_glUBOShadowCascadeFadeParamOffsets[] =
{
	offsetof( SShadowCascadeFadeBuffer, vec4ShadowCascadeFadeNear ),
	offsetof( SShadowCascadeFadeBuffer, vec4ShadowCascadeFadeFar ),
};

static GLsizeiptr g_glUBOShadowCascadeFadeParamSizes[] =
{
	sizeof( SShadowCascadeFadeBuffer().vec4ShadowCascadeFadeNear ),
	sizeof( SShadowCascadeFadeBuffer().vec4ShadowCascadeFadeFar ),
};

struct SReflectionBuffer
{
	glm::mat4 matReflectionMatrix;
};

static GLintptr g_glUBOReflectionParamOffsets[] =
{
	offsetof( SReflectionBuffer, matReflectionMatrix ),
};

static GLsizeiptr g_glUBOReflectionParamSizes[] =
{
	sizeof( SReflectionBuffer().matReflectionMatrix ),
};

static GLsizeiptr g_glUBOSizes[] =
{
	sizeof( SViewBuffer ),
	sizeof( SModelBuffer ),
	sizeof( SBonesBuffer ),
	sizeof( SClipBuffer ),
	sizeof( SLightBuffer ),
	sizeof( SLightPositionBuffer ),
	sizeof( SLightDirectionBuffer ),
	sizeof( SLightPointBuffer ),
	sizeof( SLightSpotBuffer ),
	sizeof( SLightMaxDistanceBuffer ),
	sizeof( SShadowBuffer ),
	sizeof( SShadowBlurBuffer ),
	sizeof( SShadowFadeBuffer ),
	sizeof( SShadowCascadeFadeBuffer ),
	sizeof( SReflectionBuffer ),
};

static GLintptr *g_pUBOParamOffsets[] =
{
	g_glUBOViewParamOffsets,
	g_glUBOModelParamOffsets,
	g_glUBOBonesParamOffsets,
	g_glUBOClipParamOffsets,
	g_glUBOLightParamOffsets,
	g_glUBOLightPositionParamOffsets,
	g_glUBOLightDirectionParamOffsets,
	g_glUBOLightPointParamOffsets,
	g_glUBOLightSpotParamOffsets,
	g_glUBOLightMaxDistanceParamOffsets,
	g_glUBOShadowParamOffsets,
	g_glUBOShadowBlurParamOffsets,
	g_glUBOShadowFadeParamOffsets,
	g_glUBOShadowCascadeFadeParamOffsets,
	g_glUBOReflectionParamOffsets,
};

static GLsizeiptr *g_pUBOParamSizes[] =
{
	g_glUBOViewParamSizes,
	g_glUBOModelParamSizes,
	g_glUBOBonesParamSizes,
	g_glUBOClipParamSizes,
	g_glUBOLightParamSizes,
	g_glUBOLightPositionParamSizes,
	g_glUBOLightDirectionParamSizes,
	g_glUBOLightPointParamSizes,
	g_glUBOLightSpotParamSizes,
	g_glUBOLightMaxDistanceParamSizes,
	g_glUBOShadowParamSizes,
	g_glUBOShadowBlurParamSizes,
	g_glUBOShadowFadeParamSizes,
	g_glUBOShadowCascadeFadeParamSizes,
	g_glUBOReflectionParamSizes,
};

enum class EUniformBufferObjects : EBaseEnum
{
	t_view = 0,
	t_model,
	t_bones,
	t_clip,
	t_light,
	t_lightposition,
	t_lightdirection,
	t_lightpoint,
	t_lightspot,
	t_lightmaxdistance,
	t_shadow,
	t_shadowblur,
	t_shadowfade,
	t_shadowcascadefade,
	t_reflection,

	i_count,
	i_invalid = i_count,
};

static const char *g_sUniformBufferObjects[] =
{
	"viewBuffer",
	"modelBuffer",
	"bonesBuffer",
	"clipBuffer",
	"lightBuffer",
	"lightPositionBuffer",
	"lightDirectionBuffer",
	"lightPointBuffer",
	"lightSpotBuffer",
	"lightMaxDistanceBuffer",
	"shadowBuffer",
	"shadowBlurBuffer",
	"shadowFadeBuffer",
	"shadowCascadeFadeBuffer",
	"reflectionBuffer",
};

#endif // UNIFORMBUFFEROBJECT_H