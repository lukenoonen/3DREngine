#ifndef SHADER_H
#define SHADER_H

#include "Global.h"
#include "File.h"

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

	(EBaseEnum)EShaderPreprocessorSpecular::i_count,
	(EBaseEnum)EShaderPreprocessorNormal::i_count,
	(EBaseEnum)EShaderPreprocessorCamera::i_count,
	(EBaseEnum)EShaderPreprocessorShadow::i_count,
};

class CSubShader
{
public:
	DECLARE_CLASS_NOBASE( CSubShader )

	CSubShader( const char *sVertexCode, const char *sGeometryCode, const char *sFragmentCode );
	~CSubShader();

	void Use( void ) const;
	GLint GetLocation( const char *sName );

private:
	void CheckCompileErrorsShader( GLuint glShader );

private:
	GLuint m_glID;
	std::vector<GLint> m_iUniformLocations;
	std::vector<char *> m_sUniformNames;
	std::unordered_map<const char *, GLint> m_mapUniformNameToLocation;
};

class CShader
{
public:
	DECLARE_CLASS_NOBASE( CShader )

	CShader( const char *sShaderName );
	~CShader();

	bool Success( void ) const;

	CSubShader *GetSubShader( EBaseEnum *eShaderPreprocessors ) const;

private:
	bool LoadShader( CFile *pFile, char *&sSource, std::vector<unsigned int> *uiIndices );
	bool LoadShader( CFile *pFile, char *&sSource );

	void CreateSubShaders( char *sVertexCode, char *sGeometryCode, char *sFragmentCode, std::vector<unsigned int> *uiVertexIndices, std::vector<unsigned int> *uiGeometryIndices, std::vector<unsigned int> *uiFragmentIndices, EBaseEnum eIndex, unsigned int uiIndex, unsigned int uiPreviousCount );

private:
	bool m_bSuccess;
	bool m_bHasPreprocessor[(EBaseEnum)EShaderPreprocessor::i_count];
	CSubShader *m_pSubShader[(EBaseEnum)EShaderPreprocessorQuality::i_count][(EBaseEnum)EShaderPreprocessorAnimate::i_count][(EBaseEnum)EShaderPreprocessorShadow::i_count];
	std::unordered_map<unsigned int, CSubShader *> m_mapSubShaders;

};

#endif // SHADER_H