#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "Global.h"
#include "Shader.h"

enum UniformBufferObjects_t : unsigned char
{
	UBO_VIEW = 0,
	UBO_MODEL,
	UBO_BONES,
	UBO_LIGHT,
	UBO_LIGHTPOSITION,
	UBO_LIGHTDIRECTION,
	UBO_LIGHTPOINT,
	UBO_LIGHTSPOT,
	UBO_LIGHTMAXDISTANCE,
	UBO_SHADOW,
	UBO_SHADOWBLUR,
	UBO_SHADOWFADE,
	UBO_SHADOWCASCADEFADE,
	UBO_COUNT,
};

static const char *g_sUniformBufferObjects[] =
{
	"viewBuffer",
	"modelBuffer",
	"bonesBuffer",
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
};

struct SViewBuffer
{
	glm::mat4 matProjectionView;
	glm::mat4 matProjectionViewLocked;
	glm::vec3 vecViewPos;
};

static unsigned int g_uiUBOViewParamOffsets[] =
{
	offsetof( SViewBuffer, matProjectionView ),
	offsetof( SViewBuffer, matProjectionViewLocked ),
	offsetof( SViewBuffer, vecViewPos ),
};

static unsigned int g_uiUBOViewParamSizes[] =
{
	sizeof( SViewBuffer().matProjectionView ),
	sizeof( SViewBuffer().matProjectionViewLocked ),
	sizeof( SViewBuffer().vecViewPos ),
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

struct SLightBuffer
{
	glm::vec3 vecAmbient;
	float _BUFFER0_;
	glm::vec3 vecDiffuse;
	float _BUFFER1_;
	glm::vec3 vecSpecular;
};

static unsigned int g_uiUBOLightParamOffsets[] =
{
	offsetof( SLightBuffer, vecAmbient ),
	offsetof( SLightBuffer, vecDiffuse ),
	offsetof( SLightBuffer, vecSpecular ),
};

static unsigned int g_uiUBOLightParamSizes[] =
{
	sizeof( SLightBuffer().vecAmbient ),
	sizeof( SLightBuffer().vecDiffuse ),
	sizeof( SLightBuffer().vecSpecular ),
};

struct SLightPositionBuffer
{
	glm::vec3 vecPosition;
};

static unsigned int g_uiUBOLightPositionParamOffsets[] =
{
	offsetof( SLightPositionBuffer, vecPosition ),
};

static unsigned int g_uiUBOLightPositionParamSizes[] =
{
	sizeof( SLightPositionBuffer().vecPosition ),
};

struct SLightDirectionBuffer
{
	glm::vec3 vecDirection;
};

static unsigned int g_uiUBOLightDirectionParamOffsets[] =
{
	offsetof( SLightDirectionBuffer, vecDirection ),
};

static unsigned int g_uiUBOLightDirectionParamSizes[] =
{
	sizeof( SLightDirectionBuffer().vecDirection ),
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
	glm::mat4 matLightSpaceMatrix[6];
};

static unsigned int g_uiUBOShadowParamOffsets[] =
{
	offsetof( SShadowBuffer, matLightSpaceMatrix ),
};

static unsigned int g_uiUBOShadowParamSizes[] =
{
	sizeof( SShadowBuffer().matLightSpaceMatrix[0] ),
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
	glm::vec4 vecShadowCascadeFadeNear;
	glm::vec4 vecShadowCascadeFadeFar;
};

static unsigned int g_uiUBOShadowCascadeFadeParamOffsets[] =
{
	offsetof( SShadowCascadeFadeBuffer, vecShadowCascadeFadeNear ),
	offsetof( SShadowCascadeFadeBuffer, vecShadowCascadeFadeFar ),
};

static unsigned int g_uiUBOShadowCascadeFadeParamSizes[] =
{
	sizeof( SShadowCascadeFadeBuffer().vecShadowCascadeFadeNear ),
	sizeof( SShadowCascadeFadeBuffer().vecShadowCascadeFadeFar ),
};

static unsigned int g_uiUBOSizes[] =
{
	sizeof( SViewBuffer ),
	sizeof( SModelBuffer ),
	sizeof( SBonesBuffer ),
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
};

static unsigned int *g_pUBOParamOffsets[] =
{
	g_uiUBOViewParamOffsets,
	g_uiUBOModelParamOffsets,
	g_uiUBOBonesParamOffsets,
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
};

static unsigned int *g_pUBOParamSizes[] =
{
	g_uiUBOViewParamSizes,
	g_uiUBOModelParamSizes,
	g_uiUBOBonesParamSizes,
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
};

class CShaderManager
{
public:
	DECLARE_CLASS_NOBASE( CShaderManager );

	CShaderManager();
	~CShaderManager();

	void SetUniformBufferObject( UniformBufferObjects_t tBufferObject, unsigned int uiIndex, const void *pData );
	void SetUniformBufferObject( UniformBufferObjects_t tBufferObject, unsigned int uiIndex, unsigned int uiParamIndex, unsigned int uiParams, const void *pData );

	void SetShaderQuality( ShaderQuality_t tShaderQuality );
	void SetShaderAnimate( ShaderAnimate_t tShaderAnimate );
	void SetShaderShadow( ShaderShadow_t tShaderShadow );

	void Use( ShaderType_t tShaderType );

	void SetValue( const char *sName, bool bValue );
	void SetValue( const char *sName, int iValue );
	void SetValue( const char *sName, float flValue );
	void SetValue( const char *sName, const glm::vec2 &vecValue );
	void SetValue( const char *sName, float x, float y );
	void SetValue( const char *sName, const glm::vec3 &vecValue );
	void SetValue( const char *sName, float x, float y, float z );
	void SetValue( const char *sName, const glm::vec4 &vecValue );
	void SetValue( const char *sName, float x, float y, float z, float w );
	void SetValue( const char *sName, const glm::mat2 &matValue );
	void SetValue( const char *sName, const glm::mat3 &matValue );
	void SetValue( const char *sName, const glm::mat4 &matValue );

	ShaderQuality_t GetShaderQuality( void ) const;
	ShaderAnimate_t GetShaderAnimate( void ) const;
	ShaderShadow_t GetShaderShadow( void ) const;

private:
	unsigned int m_uiUBOs[UBO_COUNT];

	CShader *m_pShaders[SHADERTYPE_COUNT];
	CSubShader *m_pActiveSubShader;

	ShaderQuality_t m_tShaderQuality;
	ShaderAnimate_t m_tShaderAnimate;
	ShaderShadow_t m_tShaderShadow;
};

#endif // SHADERMANAGER_H