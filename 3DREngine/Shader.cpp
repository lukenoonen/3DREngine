#include "Shader.h"
#include "ShaderManager.h"
#include "FileManager.h"
#include <iostream>

ShaderType_t UTIL_ShaderNameToType( const char *sShaderName )
{
	for (int i = 0; i < SHADERTYPE_COUNT; i++)
	{
		if (UTIL_streq( g_sShaderNames[i], sShaderName ))
			return (ShaderType_t)i;
	}

	return SHADERTYPE_INVALID;
}

const char *UTIL_ShaderTypeToName( ShaderType_t tShaderType )
{
	return g_sShaderNames[tShaderType];
}

CSubShader::CSubShader( const char *sVertexCode, const char *sGeometryCode, const char *sFragmentCode )
{
	unsigned int uiVertex, uiGeometry, uiFragment;

	uiVertex = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( uiVertex, 1, &sVertexCode, NULL );
	glCompileShader( uiVertex );
	CheckCompileErrorsShader( uiVertex );

	if (sGeometryCode)
	{
		uiGeometry = glCreateShader( GL_GEOMETRY_SHADER );
		glShaderSource( uiGeometry, 1, &sGeometryCode, NULL );
		glCompileShader( uiGeometry );
		CheckCompileErrorsShader( uiGeometry );
	}

	uiFragment = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( uiFragment, 1, &sFragmentCode, NULL );
	glCompileShader( uiFragment );
	CheckCompileErrorsShader( uiFragment );

	m_uiID = glCreateProgram();
	glAttachShader( m_uiID, uiVertex );
	if (sGeometryCode)
		glAttachShader( m_uiID, uiGeometry );
	glAttachShader( m_uiID, uiFragment );

	glLinkProgram( m_uiID );

	glDeleteShader( uiVertex );
	if (sGeometryCode)
		glDeleteShader( uiGeometry );

	glDeleteShader( uiFragment );

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
		else
		{
			delete[] sUniformName;
		}
	}
}

CSubShader::~CSubShader()
{
	for (unsigned int i = 0; i < m_sUniformNames.size(); i++)
		delete[] m_sUniformNames[i];

	glDeleteProgram( m_uiID );
}

void CSubShader::Use( void )
{
	glUseProgram( m_uiID );
}

GLint CSubShader::GetLocation( const char *sName )
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

void CSubShader::CheckCompileErrorsShader( unsigned int uiShader )
{
	int iSuccess;
	char sInfoLog[1024];
	glGetShaderiv( uiShader, GL_COMPILE_STATUS, &iSuccess );
	if (!iSuccess)
	{
		glGetShaderInfoLog( uiShader, 1024, NULL, sInfoLog );
		std::cout << "ERROR::SHADER::COMPILATION_ERROR of type: SHADER\n" << sInfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
}

CShader::CShader( const char *sVertexPath, const char *sGeometryPath, const char *sFragmentPath )
{
	m_bSuccess = false;

	for (unsigned int i = 0; i < (unsigned int)SHADERQUALITY_COUNT; i++)
	{
		for (unsigned int j = 0; j < (unsigned int)SHADERANIMATE_COUNT; j++)
		{
			for (unsigned int k = 0; k < (unsigned int)SHADERSHADOW_COUNT; k++)
				m_pSubShaders[i][j][k] = NULL;
		}
	}

	std::vector<unsigned int> uiVertexQualityIndices;
	std::vector<unsigned int> uiVertexAnimateIndices;
	std::vector<unsigned int> uiVertexShadowIndices;
	char *sVertexCode = ReadShaderFile( sVertexPath, uiVertexQualityIndices, uiVertexAnimateIndices, uiVertexShadowIndices );
	if (!sVertexCode)
		return;

	std::vector<unsigned int> uiGeometryQualityIndices;
	std::vector<unsigned int> uiGeometryAnimateIndices;
	std::vector<unsigned int> uiGeometryShadowIndices;
	char *sGeometryCode = ReadShaderFile( sGeometryPath, uiGeometryQualityIndices, uiGeometryAnimateIndices, uiGeometryShadowIndices );

	std::vector<unsigned int> uiFragmentQualityIndices;
	std::vector<unsigned int> uiFragmentAnimateIndices;
	std::vector<unsigned int> uiFragmentShadowIndices;
	char *sFragmentCode = ReadShaderFile( sFragmentPath, uiFragmentQualityIndices, uiFragmentAnimateIndices, uiFragmentShadowIndices );
	if (!sFragmentCode)
	{
		delete[] sVertexCode;

		if (sGeometryCode)
			delete[] sGeometryCode;

		return;
	}

	m_bHasQualityPreprocessor = uiVertexQualityIndices.empty() || uiGeometryQualityIndices.empty() || uiFragmentQualityIndices.empty();
	m_bHasAnimatePreprocessor = uiVertexAnimateIndices.empty() || uiGeometryAnimateIndices.empty() || uiFragmentAnimateIndices.empty();
	m_bHasShadowPreprocessor = uiVertexShadowIndices.empty() || uiGeometryShadowIndices.empty() || uiFragmentShadowIndices.empty();

	for (unsigned int i = 0; i < (unsigned int)SHADERQUALITY_COUNT; i++)
	{
		if (m_bHasQualityPreprocessor)
		{
			if (!uiVertexQualityIndices.empty())
				sVertexCode[uiVertexQualityIndices[i]] = '1';

			if (!uiGeometryQualityIndices.empty())
				sGeometryCode[uiGeometryQualityIndices[i]] = '1';

			if (!uiFragmentQualityIndices.empty())
				sFragmentCode[uiFragmentQualityIndices[i]] = '1';
		}

		for (unsigned int j = 0; j < (unsigned int)SHADERANIMATE_COUNT; j++)
		{
			if (m_bHasAnimatePreprocessor)
			{
				if (!uiVertexAnimateIndices.empty())
					sVertexCode[uiVertexAnimateIndices[j]] = '1';

				if (!uiGeometryAnimateIndices.empty())
					sGeometryCode[uiGeometryAnimateIndices[j]] = '1';

				if (!uiFragmentAnimateIndices.empty())
					sFragmentCode[uiFragmentAnimateIndices[j]] = '1';
			}

			for (unsigned int k = 0; k < (unsigned int)SHADERSHADOW_COUNT; k++)
			{
				if (m_bHasShadowPreprocessor)
				{
					if (!uiVertexShadowIndices.empty())
						sVertexCode[uiVertexShadowIndices[k]] = '1';

					if (!uiGeometryShadowIndices.empty())
						sGeometryCode[uiGeometryShadowIndices[k]] = '1';

					if (!uiFragmentShadowIndices.empty())
						sFragmentCode[uiFragmentShadowIndices[k]] = '1';
				}

				m_pSubShaders[i][j][k] = new CSubShader( sVertexCode, sGeometryCode, sFragmentCode );

				if (!m_bHasShadowPreprocessor)
					break;

				if (!uiVertexShadowIndices.empty())
					sVertexCode[uiVertexShadowIndices[k]] = '0';

				if (!uiGeometryShadowIndices.empty())
					sGeometryCode[uiGeometryShadowIndices[k]] = '0';

				if (!uiFragmentShadowIndices.empty())
					sFragmentCode[uiFragmentShadowIndices[k]] = '0';
			}

			if (!m_bHasAnimatePreprocessor)
				break;

			if (!uiVertexAnimateIndices.empty())
				sVertexCode[uiVertexAnimateIndices[j]] = '0';

			if (!uiGeometryAnimateIndices.empty())
				sGeometryCode[uiGeometryAnimateIndices[j]] = '0';

			if (!uiFragmentAnimateIndices.empty())
				sFragmentCode[uiFragmentAnimateIndices[j]] = '0';
		}

		if (!m_bHasQualityPreprocessor)
			break;

		if (!uiVertexQualityIndices.empty())
			sVertexCode[uiVertexQualityIndices[i]] = '0';

		if (!uiGeometryQualityIndices.empty())
			sGeometryCode[uiGeometryQualityIndices[i]] = '0';

		if (!uiFragmentQualityIndices.empty())
			sFragmentCode[uiFragmentQualityIndices[i]] = '0';
	}

	delete[] sVertexCode;
	if (sGeometryCode)
		delete[] sGeometryCode;

	delete[] sFragmentCode;

	m_bSuccess = true;
}

CShader::~CShader()
{
	for (unsigned int i = 0; i < (unsigned int)SHADERQUALITY_COUNT; i++)
	{
		for (unsigned int j = 0; j < (unsigned int)SHADERANIMATE_COUNT; j++)
		{
			for (unsigned int k = 0; k < (unsigned int)SHADERSHADOW_COUNT; k++)
			{
				if (m_pSubShaders[i][j][k])
					delete m_pSubShaders[i][j][k];
			}
		}
	}
}

CSubShader *CShader::GetSubShader( ShaderQuality_t tShaderQuality, ShaderAnimate_t tShaderAnimate, ShaderShadow_t tShaderShadow ) const
{
	return m_pSubShaders[m_bHasQualityPreprocessor ? tShaderQuality : 0][m_bHasAnimatePreprocessor ? tShaderAnimate : 0][m_bHasShadowPreprocessor ? tShaderShadow : 0];
}

bool CShader::IsSuccess( void ) const
{
	return m_bSuccess;
}

char *CShader::ReadShaderFile( const char *sPath, std::vector<unsigned int> &uiQualityIndices, std::vector<unsigned int> &uiAnimateIndices, std::vector<unsigned int> &uiShadowIndices )
{
	char *sSource = ReadShaderFile( sPath );
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
			if (UTIL_streq( "subshader", sPreProcessorChar, UTIL_Min( sizeof( "subshader" ), iPreProcessorSize ) - 1 ))
			{
				sPreProcessorChar = UTIL_strchri( sPreProcessorChar + iPreProcessorSize, " \t\n" );
				int iSubShaderSize = (int)(UTIL_strchr( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);

				if (uiQualityIndices.empty() && UTIL_streq( "QUALITY", sPreProcessorChar, UTIL_Min( sizeof( "QUALITY" ), iSubShaderSize ) - 1 ))
				{
					uiQualityIndices.resize( SHADERQUALITY_COUNT );

					const char *sSubShaderDefines;

					sSubShaderDefines = "#define QUALITY_LOW 0\n";
					while (*sSubShaderDefines) cOutput.push_back( *sSubShaderDefines++ );
					uiQualityIndices[SHADERQUALITY_LOW] = (unsigned int)cOutput.size() - 2;

					sSubShaderDefines = "#define QUALITY_MEDIUM 0\n";
					while (*sSubShaderDefines) cOutput.push_back( *sSubShaderDefines++ );
					uiQualityIndices[SHADERQUALITY_MEDIUM] = (unsigned int)cOutput.size() - 2;

					sSubShaderDefines = "#define QUALITY_HIGH 0\n";
					while (*sSubShaderDefines) cOutput.push_back( *sSubShaderDefines++ );
					uiQualityIndices[SHADERQUALITY_HIGH] = (unsigned int)cOutput.size() - 2;

					sSubShaderDefines = "#define QUALITY_ULTRA 0\n";
					while (*sSubShaderDefines) cOutput.push_back( *sSubShaderDefines++ );
					uiQualityIndices[SHADERQUALITY_ULTRA] = (unsigned int)cOutput.size() - 2;
				}
				else if (uiAnimateIndices.empty() && UTIL_streq( "ANIMATE", sPreProcessorChar, UTIL_Min( sizeof( "ANIMATE" ), iSubShaderSize ) - 1 ))
				{
					uiAnimateIndices.resize( SHADERANIMATE_COUNT );

					const char *sSubShaderDefines;

					sSubShaderDefines = "#define ANIMATE_FALSE 0\n";
					while (*sSubShaderDefines) cOutput.push_back( *sSubShaderDefines++ );
					uiAnimateIndices[SHADERANIMATE_FALSE] = (unsigned int)cOutput.size() - 2;

					sSubShaderDefines = "#define ANIMATE_TRUE 0\n";
					while (*sSubShaderDefines) cOutput.push_back( *sSubShaderDefines++ );
					uiAnimateIndices[SHADERANIMATE_TRUE] = (unsigned int)cOutput.size() - 2;
				}
				else if (uiShadowIndices.empty() && UTIL_streq( "SHADOW", sPreProcessorChar, UTIL_Min( sizeof( "SHADOW" ), iSubShaderSize ) - 1 ))
				{
					uiShadowIndices.resize( SHADERSHADOW_COUNT );

					const char *sSubShaderDefines;

					sSubShaderDefines = "#define SHADOW_FALSE 0\n";
					while (*sSubShaderDefines) cOutput.push_back( *sSubShaderDefines++ );
					uiShadowIndices[SHADERSHADOW_FALSE] = (unsigned int)cOutput.size() - 2;

					sSubShaderDefines = "#define SHADOW_TRUE 0\n";
					while (*sSubShaderDefines) cOutput.push_back( *sSubShaderDefines++ );
					uiShadowIndices[SHADERSHADOW_TRUE] = (unsigned int)cOutput.size() - 2;
				}

				sReadChar = sPreProcessorChar + iSubShaderSize + 1;
				continue;
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

char *CShader::ReadShaderFile( const char *sPath )
{
	char *sSource;
	if (!pFileManager->ReadEntireFile( sPath, sSource ))
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

				char *sIncludeSource = ReadShaderFile( sPathName );

				if (sIncludeSource)
				{
					const char *sIncludeSourceReadChar = sIncludeSource;
					while (*sIncludeSourceReadChar)
						cOutput.push_back( *sIncludeSourceReadChar++ );

					delete[] sIncludeSource;

					sReadChar = sPreProcessorChar + iPathSize + 2;
					cOutput.push_back( '\n' );
				}
				else
				{
					std::cout << "ERROR::SHADER::PREPROCESSOR_ERROR of type: SHADER (" << sPath << ")\n" << "Can't include file '" << sPathName << "'!\n -- --------------------------------------------------- -- " << std::endl;

					sReadChar = sPreProcessorChar + iPathSize + 2;
				}

				delete[] sPathName;
				continue;
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