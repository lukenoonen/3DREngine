#include "Shader.h"
#include <UTIL.h>

#include <glad/glad.h>
#include <iostream>

#include "ShaderManager.h"

CShader::CShader( const char *sVertexPath, const char *sGeometryPath, const char *sFragmentPath, ShaderSubType_t tShaderSubType, ShaderQuality_t tShaderQuality )
{
	m_bSuccess = false;

	char *sVertexCode = ReadShaderFile( sVertexPath, tShaderSubType, tShaderQuality );
	char *sGeometryCode = ReadShaderFile( sGeometryPath, tShaderSubType, tShaderQuality );
	char *sFragmentCode = ReadShaderFile( sFragmentPath, tShaderSubType, tShaderQuality );

	if (!sVertexCode)
		return;

	if (!sFragmentCode)
		return;

	unsigned int uiVertex, uiGeometry, uiFragment;

	uiVertex = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( uiVertex, 1, &sVertexCode, NULL );
	glCompileShader( uiVertex );
	CheckCompileErrorsShader( uiVertex, sVertexPath );

	if (sGeometryCode)
	{
		uiGeometry = glCreateShader( GL_GEOMETRY_SHADER );
		glShaderSource( uiGeometry, 1, &sGeometryCode, NULL );
		glCompileShader( uiGeometry );
		CheckCompileErrorsShader( uiGeometry, sGeometryPath );
	}

	uiFragment = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( uiFragment, 1, &sFragmentCode, NULL );
	glCompileShader( uiFragment );
	CheckCompileErrorsShader( uiFragment, sFragmentPath );

	m_uiID = glCreateProgram();
	glAttachShader( m_uiID, uiVertex );
	if (sGeometryCode)
		glAttachShader( m_uiID, uiGeometry );
	glAttachShader( m_uiID, uiFragment );
	glLinkProgram( m_uiID );
	CheckCompileErrorsProgram( m_uiID );

	glDeleteShader( uiVertex );
	if (sGeometryCode)
		glDeleteShader( uiGeometry );
	glDeleteShader( uiFragment );

	delete[] sVertexCode;
	if (sGeometryCode)
		delete[] sGeometryCode;
	delete[] sFragmentCode;

	for (unsigned int i = 0; i < UBO_COUNT; i++)
	{
		unsigned int uiIndex = glGetUniformBlockIndex( m_uiID, g_sUniformBufferObjects[i] );
		if (uiIndex != GL_INVALID_INDEX)
			glUniformBlockBinding( m_uiID, uiIndex, i );
	}

	GLint iUniformCount;
	GLint iMaxUniformLength;

	glGetProgramiv( m_uiID, GL_ACTIVE_UNIFORMS, &iUniformCount );
	glGetProgramiv( m_uiID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &iMaxUniformLength );

	for (GLint i = 0; i < iUniformCount; i++)
	{
		GLsizei iLength;
		GLint iSize;
		GLenum tType;
		GLchar *sUniformName = new GLchar[iMaxUniformLength];

		glGetActiveUniform( m_uiID, i, iMaxUniformLength, &iLength, &iSize, &tType, sUniformName );
		GLint iLocation = glGetUniformLocation( m_uiID, sUniformName );

		if (iLocation != -1)
		{
			m_iUniformLocations.push_back( iLocation );
			m_sUniformNames.push_back( sUniformName );
			if (iSize > 1)
			{
				GLchar *sUniformNameEdit = UTIL_stredit( sUniformName );
				*(UTIL_strchr( sUniformNameEdit, '[' ) + 1) = '\0';
				for (GLint j = 1; j < iSize; j++)
				{
					GLchar *sModifiedUniformName = new GLchar[iMaxUniformLength];
					sprintf_s( sModifiedUniformName, iMaxUniformLength, "%s%d]", sUniformNameEdit, j );
					m_iUniformLocations.push_back( glGetUniformLocation( m_uiID, sModifiedUniformName ) );
					m_sUniformNames.push_back( sModifiedUniformName );
				}

				delete[] sUniformNameEdit;
			}
		}
	}

	m_bSuccess = true;
}

CShader::~CShader()
{
	for (unsigned int i = 0; i < m_sUniformNames.size(); i++)
		delete[] m_sUniformNames[i];

	glDeleteProgram( m_uiID );
}

void CShader::Use( void )
{
	if (pShaderManager->GetActiveShader() != this)
	{
		glUseProgram( m_uiID );
		pShaderManager->SetActiveShader( this );
	}
}

void CShader::SetValue( const char *sName, bool bValue )
{
	glUniform1i( GetLocation( sName ), (int)bValue );
}

void CShader::SetValue( const char *sName, int iValue )
{
	glUniform1i( GetLocation( sName ), iValue );
}

void CShader::SetValue( const char *sName, float flValue )
{
	glUniform1f( GetLocation( sName ), flValue );
}

void CShader::SetValue( const char *sName, const glm::vec2 &vecValue )
{
	glUniform2fv( GetLocation( sName ), 1, &vecValue[0] );
}

void CShader::SetValue( const char *sName, float x, float y )
{
	glUniform2f( GetLocation( sName ), x, y );
}

void CShader::SetValue( const char *sName, const glm::vec3 &vecValue )
{
	glUniform3fv( GetLocation( sName ), 1, &vecValue[0] );
}

void CShader::SetValue( const char *sName, float x, float y, float z )
{
	glUniform3f( GetLocation( sName ), x, y, z );
}

void CShader::SetValue( const char *sName, const glm::vec4 &vecValue )
{
	glUniform4fv( GetLocation( sName ), 1, &vecValue[0] );
}

void CShader::SetValue( const char *sName, float x, float y, float z, float w )
{
	glUniform4f( GetLocation( sName ), x, y, z, w );
}

void CShader::SetValue( const char *sName, const glm::mat2 &matValue )
{
	glUniformMatrix2fv( GetLocation( sName ), 1, GL_FALSE, &matValue[0][0] );
}

void CShader::SetValue( const char *sName, const glm::mat3 &matValue )
{
	glUniformMatrix3fv( GetLocation( sName ), 1, GL_FALSE, &matValue[0][0] );
}

void CShader::SetValue( const char *sName, const glm::mat4 &matValue )
{
	glUniformMatrix4fv( GetLocation( sName ), 1, GL_FALSE, &matValue[0][0] );
}

bool CShader::IsSuccess( void ) const
{
	return m_bSuccess;
}

GLint CShader::GetLocation( const char *sName )
{
	std::unordered_map<const char *, GLint>::iterator itUniformSearch = m_mapUniformNameToLocation.find( sName );
	if (itUniformSearch == m_mapUniformNameToLocation.end())
	{
		for (unsigned int i = 0; i < m_sUniformNames.size(); i++)
		{
			if (UTIL_strcmp( sName, m_sUniformNames[i] ) == 0)
			{
				GLint iUniformLocation = m_iUniformLocations[i];
				m_mapUniformNameToLocation.emplace( sName, iUniformLocation );
				return iUniformLocation;
			}
		}

		return -1;
	}

	return itUniformSearch->second;
}

char *CShader::ReadShaderFile( const char *sPath, ShaderSubType_t tShaderSubType, ShaderQuality_t tShaderQuality )
{
	char *sSource = ReadShaderFile( sPath );
	if (!sSource)
		return NULL;

	const char *sReadChar = sSource;

	std::vector<char> cOutput;

	bool bHasSubTypePreProcessor = false;
	bool bHasCorrectSubTypePreProcessor = false;
	bool bHasQualityPreProcessor = false;
	bool bHasCorrectQualityPreProcessor = false;

	while (*sReadChar)
	{
		if (*sReadChar == '#')
		{
			const char *sPreProcessorChar = sReadChar + 1;
			int iPreProcessorSize = (int)(UTIL_strchr( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);
			if (UTIL_strcmp( "subtype", sPreProcessorChar, UTIL_Min( sizeof( "subtype" ), iPreProcessorSize ) - 1 ) == 0)
			{
				bHasSubTypePreProcessor = true;

				sPreProcessorChar = UTIL_strchri( sPreProcessorChar + iPreProcessorSize, " \t\n" );
				int iSubTypeSize = (int)(UTIL_strchr( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);

				if (!bHasCorrectSubTypePreProcessor)
				{
					const char *sSubTypeDefines = NULL;
					switch (tShaderSubType)
					{
					case SHADERSUBTYPE_DEFAULT:
						if (UTIL_strcmp( "SUBTYPE_DEFAULT", sPreProcessorChar, UTIL_Min( sizeof( "SUBTYPE_DEFAULT" ), iSubTypeSize ) - 1 ) == 0)
							sSubTypeDefines = "#define SUBTYPE_DEFAULT 1";
						break;
					case SHADERSUBTYPE_DIR:
						if (UTIL_strcmp( "SUBTYPE_DIR", sPreProcessorChar, UTIL_Min( sizeof( "SUBTYPE_DIR" ), iSubTypeSize ) - 1 ) == 0)
							sSubTypeDefines = "#define SUBTYPE_DIR 1";
						break;
					case SHADERSUBTYPE_POINT:
						if (UTIL_strcmp( "SUBTYPE_POINT", sPreProcessorChar, UTIL_Min( sizeof( "SUBTYPE_POINT" ), iSubTypeSize ) - 1 ) == 0)
							sSubTypeDefines = "#define SUBTYPE_POINT 1";
						break;
					case SHADERSUBTYPE_SPOT:
						if (UTIL_strcmp( "SUBTYPE_SPOT", sPreProcessorChar, UTIL_Min( sizeof( "SUBTYPE_SPOT" ), iSubTypeSize ) - 1 ) == 0)
							sSubTypeDefines = "#define SUBTYPE_SPOT 1";
						break;
					case SHADERSUBTYPE_CSM:
						if (UTIL_strcmp( "SUBTYPE_CSM", sPreProcessorChar, UTIL_Min( sizeof( "SUBTYPE_CSM" ), iSubTypeSize ) - 1 ) == 0)
							sSubTypeDefines = "#define SUBTYPE_CSM 1";
						break;
					}

					if (sSubTypeDefines)
					{
						bHasCorrectSubTypePreProcessor = true;

						while (*sSubTypeDefines)
							cOutput.push_back( *sSubTypeDefines++ );

						cOutput.push_back( '\n' );
					}
				}

				sReadChar = sPreProcessorChar + iSubTypeSize + 1;
				continue;
			}
			else if (UTIL_strcmp( "quality", sPreProcessorChar, UTIL_Min( sizeof( "quality" ), iPreProcessorSize ) - 1 ) == 0)
			{
				bHasQualityPreProcessor = true;

				sPreProcessorChar = UTIL_strchri( sPreProcessorChar + iPreProcessorSize, " \t\n" );
				int iQualitySize = (int)(UTIL_strchr( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);

				if (!bHasCorrectQualityPreProcessor)
				{
					const char *sQualityDefines = NULL;
					switch (tShaderQuality)
					{
					case SHADERQUALITY_DEFAULT:
						if (UTIL_strcmp( "QUALITY_DEFAULT", sPreProcessorChar, UTIL_Min( sizeof( "QUALITY_DEFAULT" ), iQualitySize ) - 1 ) == 0)
							sQualityDefines = "#define QUALITY_DEFAULT 1";
						break;
					case SHADERQUALITY_LOW:
						if (UTIL_strcmp( "QUALITY_LOW", sPreProcessorChar, UTIL_Min( sizeof( "QUALITY_LOW" ), iQualitySize ) - 1 ) == 0)
							sQualityDefines = "#define QUALITY_LOW 1";
						break;
					case SHADERQUALITY_MEDIUM:
						if (UTIL_strcmp( "QUALITY_MEDIUM", sPreProcessorChar, UTIL_Min( sizeof( "QUALITY_MEDIUM" ), iQualitySize ) - 1 ) == 0)
							sQualityDefines = "#define QUALITY_MEDIUM 1";
						break;
					case SHADERQUALITY_HIGH:
						if (UTIL_strcmp( "QUALITY_HIGH", sPreProcessorChar, UTIL_Min( sizeof( "QUALITY_HIGH" ), iQualitySize ) - 1 ) == 0)
							sQualityDefines = "#define QUALITY_HIGH 1";
						break;
					case SHADERQUALITY_ULTRA:
						if (UTIL_strcmp( "QUALITY_ULTRA", sPreProcessorChar, UTIL_Min( sizeof( "QUALITY_ULTRA" ), iQualitySize ) - 1 ) == 0)
							sQualityDefines = "#define QUALITY_ULTRA 1";
						break;
					}

					if (sQualityDefines)
					{
						bHasCorrectQualityPreProcessor = true;

						while (*sQualityDefines)
							cOutput.push_back( *sQualityDefines++ );

						cOutput.push_back( '\n' );
					}
				}

				sReadChar = sPreProcessorChar + iQualitySize + 1;
				continue;
			}
		}

		cOutput.push_back( *sReadChar++ );
	}

	delete[] sSource;

	if ((tShaderSubType != SHADERSUBTYPE_DEFAULT || bHasSubTypePreProcessor) && !bHasCorrectSubTypePreProcessor)
		return NULL;

	if ((tShaderQuality != SHADERQUALITY_DEFAULT || bHasQualityPreProcessor) && !bHasCorrectQualityPreProcessor)
		return NULL;

	unsigned int uiSize = (unsigned int)cOutput.size();
	char *sOutput = new char[uiSize + 1];
	for (unsigned int i = 0; i < uiSize; i++)
		sOutput[i] = cOutput[i];

	sOutput[uiSize] = '\0';

	return sOutput;
}

char *CShader::ReadShaderFile( const char *sPath )
{
	char *sSource = UTIL_readf( sPath );
	if (!sSource)
		return NULL;

	const char *sReadChar = sSource;

	std::vector<char> cOutput;

	while (*sReadChar)
	{
		if (*sReadChar == '#')
		{
			const char *sPreProcessorChar = sReadChar + 1;
			int iPreProcessorSize = (int)(UTIL_strchr( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);
			if (UTIL_strcmp( "include", sPreProcessorChar, UTIL_Min( sizeof( "include" ), iPreProcessorSize ) - 1 ) == 0)
			{
				sPreProcessorChar = UTIL_strchr( sPreProcessorChar + iPreProcessorSize, '\"' ) + 1;
				int iPathSize = (int)(UTIL_strchr( sPreProcessorChar, '\"' ) - sPreProcessorChar);

				char *sPathName = new char[iPathSize + 1];
				UTIL_strncpy( sPathName, sPreProcessorChar, iPathSize );
				sPathName[iPathSize] = '\0';

				char sFullPathName[260];
				sprintf_s( sFullPathName, sizeof( sFullPathName ), "resources/shaders/%s", sPathName );

				char *sIncludeSource = ReadShaderFile( sFullPathName );

				delete[] sPathName;

				if (sIncludeSource)
				{
					const char *sIncludeSourceReadChar = sIncludeSource;
					while (*sIncludeSourceReadChar)
						cOutput.push_back( *sIncludeSourceReadChar++ );

					delete[] sIncludeSource;

					sReadChar = sPreProcessorChar + iPathSize + 2;
					cOutput.push_back( '\n' );
					continue;
				}
				else
				{
					std::cout << "ERROR::SHADER::PREPROCESSOR_ERROR of type: SHADER (" << sPath << ")\n" << "Can't include file '" << sFullPathName << "'!\n -- --------------------------------------------------- -- " << std::endl;

					sReadChar = sPreProcessorChar + iPathSize + 2;
					continue;
				}
			}
		}

		cOutput.push_back( *sReadChar++ );
	}

	delete[] sSource;

	unsigned int uiSize = (unsigned int)cOutput.size();
	char *sOutput = new char[uiSize + 1];
	for (unsigned int i = 0; i < uiSize; i++)
		sOutput[i] = cOutput[i];

	sOutput[uiSize] = '\0';

	return sOutput;
}

void CShader::CheckCompileErrorsShader( unsigned int iShader, const char *sPath )
{
	int iSuccess;
	char sInfoLog[1024];
	glGetShaderiv( iShader, GL_COMPILE_STATUS, &iSuccess );
	if (!iSuccess)
	{
		glGetShaderInfoLog( iShader, 1024, NULL, sInfoLog );
		std::cout << "ERROR::SHADER::COMPILATION_ERROR of type: SHADER (" << sPath << ")\n" << sInfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}

void CShader::CheckCompileErrorsProgram( unsigned int iProgram )
{
	int iSuccess;
	char sInfoLog[1024];
	glGetShaderiv( iProgram, GL_LINK_STATUS, &iSuccess );
	if (!iSuccess)
	{
		glGetShaderInfoLog( iProgram, 1024, NULL, sInfoLog );
		std::cout << "ERROR::SHADER::COMPILATION_ERROR of type: PROGRAM" << "\n" << sInfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}