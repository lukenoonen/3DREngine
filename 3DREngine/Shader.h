#ifndef SHADER_H
#define SHADER_H

#include "Global.h"

enum ShaderType_t : unsigned char
{
	SHADERTYPE_SHADOW_DIR = 0,
	SHADERTYPE_SHADOW_POINT,
	SHADERTYPE_SHADOW_SPOT,
	SHADERTYPE_SHADOW_CSM,
	SHADERTYPE_DEPTH,
	SHADERTYPE_UNLIT,
	SHADERTYPE_LIT_DIR,
	SHADERTYPE_LIT_POINT,
	SHADERTYPE_LIT_SPOT,
	SHADERTYPE_LIT_CSM,
	SHADERTYPE_SKYBOX,
	SHADERTYPE_COUNT,
	SHADERTYPE_INVALID = SHADERTYPE_COUNT,
};

static const char *g_sShaderNames[] =
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

ShaderType_t UTIL_ShaderNameToType( const char *sShaderName );
const char *UTIL_ShaderTypeToName( ShaderType_t tShaderType );

enum ShaderQuality_t : unsigned char
{
	SHADERQUALITY_LOW = 0,
	SHADERQUALITY_MEDIUM,
	SHADERQUALITY_HIGH,
	SHADERQUALITY_ULTRA,
	SHADERQUALITY_COUNT,
};

enum ShaderAnimate_t : unsigned char
{
	SHADERANIMATE_FALSE = 0,
	SHADERANIMATE_TRUE,
	SHADERANIMATE_COUNT,
};

enum ShaderShadow_t : unsigned char
{
	SHADERSHADOW_FALSE = 0,
	SHADERSHADOW_TRUE,
	SHADERSHADOW_COUNT,
};

class CSubShader
{
public:
	DECLARE_CLASS_NOBASE( CSubShader );

	CSubShader( const char *sVertexCode, const char *sGeometryCode, const char *sFragmentCode );
	~CSubShader();

	void Use( void );
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

	CSubShader *GetSubShader( ShaderQuality_t tShaderQuality, ShaderAnimate_t tShaderAnimate, ShaderShadow_t tShaderShadow ) const;

private:
	char *ReadShaderFile( const char *sPath, std::vector<unsigned int> &uiQualityIndices, std::vector<unsigned int> &uiAnimateIndices, std::vector<unsigned int> &uiShadowIndices );
	char *ReadShaderFile( const char *sPath );

private:
	bool m_bSuccess;	
	bool m_bHasQualityPreprocessor;
	bool m_bHasAnimatePreprocessor;
	bool m_bHasShadowPreprocessor;
	CSubShader *m_pSubShaders[SHADERQUALITY_COUNT][SHADERANIMATE_COUNT][SHADERSHADOW_COUNT];
};

#endif // SHADER_H