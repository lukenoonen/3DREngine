#ifndef SHADER_H
#define SHADER_H

#include "Global.h"
#include <SharedDefs.h>

class CShader
{
public:
	DECLARE_CLASS_NOBASE( CShader );

	CShader( const char *sVertexPath, const char *sGeometryPath, const char *sFragmentPath, ShaderSubType_t tShaderSubType, ShaderQuality_t tShaderQuality );
	~CShader();

	void Use( void );

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

	bool IsSuccess( void ) const;

private:
	GLint GetLocation( const char *sName );

	char *ReadShaderFile( const char *sPath, ShaderSubType_t tShaderSubType, ShaderQuality_t tShaderQuality );
	char *ReadShaderFile( const char *sPath );

	void CheckCompileErrorsShader( unsigned int iShader, const char *sPath );
	void CheckCompileErrorsProgram( unsigned int iProgram );

	unsigned int m_uiID;

	bool m_bSuccess;

	std::vector<GLint> m_iUniformLocations;
	std::vector<char *> m_sUniformNames;
	std::unordered_map<const char *, GLint> m_mapUniformNameToLocation;
};

#endif // SHADER_H