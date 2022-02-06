#ifndef SHADER_H
#define SHADER_H

#include "Global.h"

enum class EShaderType : EBaseEnum
{
	t_shadowdir = 0,
	t_shadowpoint,
	t_shadowspot,
	t_shadowcsm,
	t_depth,
	t_unlit,
	t_litdir,
	t_litpoint,
	t_litspot,
	t_litcsm,
	t_skybox,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderTypeNames[] =
{
	"shadowdir",
	"shadowpoint",
	"shadowspot",
	"shadowcsm",
	"depth",
	"unlit",
	"litdir",
	"litpoint",
	"litspot",
	"litcsm",
	"skybox",
};

enum class EShaderPreprocessorQuality : EBaseEnum
{
	t_low = 0,
	t_medium,
	t_high,
	t_ultra,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderQualityDefines[] =
{
	"#define QUALITY_LOW 0\n",
	"#define QUALITY_MEDIUM 0\n",
	"#define QUALITY_HIGH 0\n",
	"#define QUALITY_ULTRA 0\n",
};

enum class EShaderPreprocessorAnimate : EBaseEnum
{
	t_false = 0,
	t_true,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderAnimateDefines[] =
{
	"#define ANIMATE_FALSE 0\n",
	"#define ANIMATE_TRUE 0\n",
};

enum class EShaderPreprocessorClip : EBaseEnum
{
	t_false = 0,
	t_true,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderClipDefines[] =
{
	"#define CLIP_FALSE 0\n",
	"#define CLIP_TRUE 0\n",
};

enum class EShaderPreprocessorReflection : EBaseEnum
{
	t_false = 0,
	t_true,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderReflectionDefines[] =
{
	"#define REFLECTION_FALSE 0\n",
	"#define REFLECTION_TRUE 0\n",
};

enum class EShaderPreprocessorSpecular : EBaseEnum
{
	t_false = 0,
	t_true,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderSpecularDefines[] =
{
	"#define SPECULAR_FALSE 0\n",
	"#define SPECULAR_TRUE 0\n",
};

enum class EShaderPreprocessorNormal : EBaseEnum
{
	t_false = 0,
	t_true,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderNormalDefines[] =
{
	"#define NORMAL_FALSE 0\n",
	"#define NORMAL_TRUE 0\n",
};

enum class EShaderPreprocessorCamera : EBaseEnum
{
	t_false = 0,
	t_true,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderCameraDefines[] =
{
	"#define CAMERA_FALSE 0\n",
	"#define CAMERA_TRUE 0\n",
};

enum class EShaderPreprocessorShadow : EBaseEnum
{
	t_false = 0,
	t_true,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderShadowDefines[] =
{
	"#define SHADOW_FALSE 0\n",
	"#define SHADOW_TRUE 0\n",
};

enum class EShaderPreprocessor : EBaseEnum
{
	t_quality = 0,
	t_animate,
	t_clip,
	t_reflection,

	t_specular,
	t_normal,
	t_camera,
	t_shadow,

	i_count,
	i_invalid = i_count,
};

static const char *g_sShaderPreprocessorNames[] =
{
	"QUALITY",
	"ANIMATE",
	"CLIP",
	"REFLECTION",

	"SPECULAR",
	"NORMAL",
	"CAMERA",
	"SHADOW",
};

static const char **g_pShaderPreprocessorDefines[] =
{
	g_sShaderQualityDefines,
	g_sShaderAnimateDefines,
	g_sShaderClipDefines,
	g_sShaderReflectionDefines,

	g_sShaderSpecularDefines,
	g_sShaderNormalDefines,
	g_sShaderCameraDefines,
	g_sShaderShadowDefines,
};

static const EBaseEnum g_eShaderPreprocessorCount[] =
{
	(EBaseEnum)EShaderPreprocessorQuality::i_count,
	(EBaseEnum)EShaderPreprocessorAnimate::i_count,
	(EBaseEnum)EShaderPreprocessorClip::i_count,
	(EBaseEnum)EShaderPreprocessorReflection::i_count,

	(EBaseEnum)EShaderPreprocessorSpecular ::i_count,
	(EBaseEnum)EShaderPreprocessorNormal::i_count,
	(EBaseEnum)EShaderPreprocessorCamera::i_count,
	(EBaseEnum)EShaderPreprocessorShadow::i_count,
};

struct SViewBuffer
{
	glm::mat4 matProjectionView;
	glm::mat4 matProjectionViewLocked;
	glm::vec3 vec3ViewPos;
};

static unsigned int g_uiUBOViewParamOffsets[] =
{
	offsetof( SViewBuffer, matProjectionView ),
	offsetof( SViewBuffer, matProjectionViewLocked ),
	offsetof( SViewBuffer, vec3ViewPos ),
};

static unsigned int g_uiUBOViewParamSizes[] =
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

static unsigned int g_uiUBOModelParamOffsets[] =
{
	offsetof( SModelBuffer, matModel ),
	offsetof( SModelBuffer, matTIModel ),
};

static unsigned int g_uiUBOModelParamSizes[] =
{
	sizeof( SModelBuffer().matModel ),
	sizeof( SModelBuffer().matTIModel ),
};

struct SBonesBuffer
{
	glm::mat4 matBones[64];
};

static unsigned int g_uiUBOBonesParamOffsets[] =
{
	offsetof( SBonesBuffer, matBones ),
};

static unsigned int g_uiUBOBonesParamSizes[] =
{
	sizeof( SBonesBuffer().matBones[0] ),
};

struct SClipBuffer
{
	glm::vec4 vec4ClipPlane;
};

static unsigned int g_uiUBOClipParamOffsets[] =
{
	offsetof( SClipBuffer, vec4ClipPlane ),
};

static unsigned int g_uiUBOClipParamSizes[] =
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

static unsigned int g_uiUBOLightParamOffsets[] =
{
	offsetof( SLightBuffer, vec3Ambient ),
	offsetof( SLightBuffer, vec3Diffuse ),
	offsetof( SLightBuffer, vec3Specular ),
};

static unsigned int g_uiUBOLightParamSizes[] =
{
	sizeof( SLightBuffer().vec3Ambient ),
	sizeof( SLightBuffer().vec3Diffuse ),
	sizeof( SLightBuffer().vec3Specular ),
};

struct SLightPositionBuffer
{
	glm::vec3 vec3Position;
};

static unsigned int g_uiUBOLightPositionParamOffsets[] =
{
	offsetof( SLightPositionBuffer, vec3Position ),
};

static unsigned int g_uiUBOLightPositionParamSizes[] =
{
	sizeof( SLightPositionBuffer().vec3Position ),
};

struct SLightDirectionBuffer
{
	glm::vec3 vec3Direction;
};

static unsigned int g_uiUBOLightDirectionParamOffsets[] =
{
	offsetof( SLightDirectionBuffer, vec3Direction ),
};

static unsigned int g_uiUBOLightDirectionParamSizes[] =
{
	sizeof( SLightDirectionBuffer().vec3Direction ),
};

struct SLightPointBuffer
{
	float flConstant;
	float flLinear;
	float flQuadratic;
};

static unsigned int g_uiUBOLightPointParamOffsets[] =
{
	offsetof( SLightPointBuffer, flConstant ),
	offsetof( SLightPointBuffer, flLinear ),
	offsetof( SLightPointBuffer, flQuadratic ),
};

static unsigned int g_uiUBOLightPointParamSizes[] =
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

static unsigned int g_uiUBOLightSpotParamOffsets[] =
{
	offsetof( SLightSpotBuffer, flCutOff ),
	offsetof( SLightSpotBuffer, flOuterCutOff ),
};

static unsigned int g_uiUBOLightSpotParamSizes[] =
{
	sizeof( SLightSpotBuffer().flCutOff ),
	sizeof( SLightSpotBuffer().flOuterCutOff ),
};

struct SLightMaxDistanceBuffer
{
	float flMaxDistance;
};

static unsigned int g_uiUBOLightMaxDistanceParamOffsets[] =
{
	offsetof( SLightMaxDistanceBuffer, flMaxDistance ),
};

static unsigned int g_uiUBOLightMaxDistanceParamSizes[] =
{
	sizeof( SLightMaxDistanceBuffer().flMaxDistance ),
};

struct SShadowBuffer
{
	glm::mat4 matShadowMatrix[6];
};

static unsigned int g_uiUBOShadowParamOffsets[] =
{
	offsetof( SShadowBuffer, matShadowMatrix ),
};

static unsigned int g_uiUBOShadowParamSizes[] =
{
	sizeof( SShadowBuffer().matShadowMatrix[0] ),
};

struct SShadowBlurBuffer
{
	float flBlurScale;
};

static unsigned int g_uiUBOShadowBlurParamOffsets[] =
{
	offsetof( SShadowBlurBuffer, flBlurScale ),
};

static unsigned int g_uiUBOShadowBlurParamSizes[] =
{
	sizeof( SShadowBlurBuffer().flBlurScale ),
};

struct SShadowFadeBuffer
{
	float flShadowFadeNear;
	float flShadowFadeFar;
};

static unsigned int g_uiUBOShadowFadeParamOffsets[] =
{
	offsetof( SShadowFadeBuffer, flShadowFadeNear ),
	offsetof( SShadowFadeBuffer, flShadowFadeFar ),
};

static unsigned int g_uiUBOShadowFadeParamSizes[] =
{
	sizeof( SShadowFadeBuffer().flShadowFadeNear ),
	sizeof( SShadowFadeBuffer().flShadowFadeFar ),
};

struct SShadowCascadeFadeBuffer
{
	glm::vec4 vec4ShadowCascadeFadeNear;
	glm::vec4 vec4ShadowCascadeFadeFar;
};

static unsigned int g_uiUBOShadowCascadeFadeParamOffsets[] =
{
	offsetof( SShadowCascadeFadeBuffer, vec4ShadowCascadeFadeNear ),
	offsetof( SShadowCascadeFadeBuffer, vec4ShadowCascadeFadeFar ),
};

static unsigned int g_uiUBOShadowCascadeFadeParamSizes[] =
{
	sizeof( SShadowCascadeFadeBuffer().vec4ShadowCascadeFadeNear ),
	sizeof( SShadowCascadeFadeBuffer().vec4ShadowCascadeFadeFar ),
};

struct SReflectionBuffer
{
	glm::mat4 matReflectionMatrix;
};

static unsigned int g_uiUBOReflectionParamOffsets[] =
{
	offsetof( SReflectionBuffer, matReflectionMatrix ),
};

static unsigned int g_uiUBOReflectionParamSizes[] =
{
	sizeof( SReflectionBuffer().matReflectionMatrix ),
};

static unsigned int g_uiUBOSizes[] =
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

static unsigned int *g_pUBOParamOffsets[] =
{
	g_uiUBOViewParamOffsets,
	g_uiUBOModelParamOffsets,
	g_uiUBOBonesParamOffsets,
	g_uiUBOClipParamOffsets,
	g_uiUBOLightParamOffsets,
	g_uiUBOLightPositionParamOffsets,
	g_uiUBOLightDirectionParamOffsets,
	g_uiUBOLightPointParamOffsets,
	g_uiUBOLightSpotParamOffsets,
	g_uiUBOLightMaxDistanceParamOffsets,
	g_uiUBOShadowParamOffsets,
	g_uiUBOShadowBlurParamOffsets,
	g_uiUBOShadowFadeParamOffsets,
	g_uiUBOShadowCascadeFadeParamOffsets,
	g_uiUBOReflectionParamOffsets,
};

static unsigned int *g_pUBOParamSizes[] =
{
	g_uiUBOViewParamSizes,
	g_uiUBOModelParamSizes,
	g_uiUBOBonesParamSizes,
	g_uiUBOClipParamSizes,
	g_uiUBOLightParamSizes,
	g_uiUBOLightPositionParamSizes,
	g_uiUBOLightDirectionParamSizes,
	g_uiUBOLightPointParamSizes,
	g_uiUBOLightSpotParamSizes,
	g_uiUBOLightMaxDistanceParamSizes,
	g_uiUBOShadowParamSizes,
	g_uiUBOShadowBlurParamSizes,
	g_uiUBOShadowFadeParamSizes,
	g_uiUBOShadowCascadeFadeParamSizes,
	g_uiUBOReflectionParamSizes,
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

class CSubShader
{
public:
	DECLARE_CLASS_NOBASE( CSubShader );

	CSubShader( const char *sVertexCode, const char *sGeometryCode, const char *sFragmentCode );
	~CSubShader();

	void Use( void ) const;
	GLint GetLocation( const char *sName );

private:
	void CheckCompileErrorsShader( unsigned int uiShader );

private:
	unsigned int m_uiID;
	std::vector<GLint> m_iUniformLocations;
	std::vector<char *> m_sUniformNames;
	std::unordered_map<const char *, GLint> m_mapUniformNameToLocation;
};

class CShader
{
public:
	DECLARE_CLASS_NOBASE( CShader );

	CShader( const char *sVertexPath, const char *sGeometryPath, const char *sFragmentPath );
	~CShader();

	bool IsSuccess( void ) const;

	CSubShader *GetSubShader( EBaseEnum *eShaderPreprocessors ) const;

private:
	void CreateSubShaders( char *sVertexCode, char *sGeometryCode, char *sFragmentCode, std::vector<unsigned int> *uiVertexIndices, std::vector<unsigned int> *uiGeometryIndices, std::vector<unsigned int> *uiFragmentIndices, EBaseEnum eIndex, unsigned int uiIndex, unsigned int uiPreviousCount );

	char *ReadShaderFile( const char *sPath, std::vector<unsigned int> *uiIndices );
	char *ReadShaderFile( const char *sPath );

private:
	bool m_bSuccess;
	bool m_bHasPreprocessor[(EBaseEnum)EShaderPreprocessor::i_count];
	CSubShader *m_pSubShader[(EBaseEnum)EShaderPreprocessorQuality::i_count][(EBaseEnum)EShaderPreprocessorAnimate::i_count][(EBaseEnum)EShaderPreprocessorShadow::i_count];
	std::unordered_map<unsigned int, CSubShader *> m_mapSubShaders;

};

#endif // SHADER_H