#include "Shader.h"
#include "FileManager.h"
#include "RenderManager.h"
#include <iostream>

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

	for (EBaseEnum i = 0; i < (EBaseEnum)EUniformBufferObjects::i_count; i++)
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

void CSubShader::Use( void ) const
{
	glUseProgram( m_uiID );
}

GLint CSubShader::GetLocation( const char *sName )
{
	std::unordered_map<const char *, GLint>::const_iterator itUniformSearch = m_mapUniformNameToLocation.find( sName );
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

	std::vector<unsigned int> uiVertexIndices[(EBaseEnum)EShaderPreprocessor::i_count];
	char *sVertexCode = ReadShaderFile( sVertexPath, uiVertexIndices );
	if (!sVertexCode)
		return;

	std::vector<unsigned int> uiGeometryIndices[(EBaseEnum)EShaderPreprocessor::i_count];
	char *sGeometryCode = ReadShaderFile( sGeometryPath, uiGeometryIndices );

	std::vector<unsigned int> uiFragmentIndices[(EBaseEnum)EShaderPreprocessor::i_count];
	char *sFragmentCode = ReadShaderFile( sFragmentPath, uiFragmentIndices );
	if (!sFragmentCode)
	{
		delete[] sVertexCode;

		if (sGeometryCode)
			delete[] sGeometryCode;

		return;
	}

	for (EBaseEnum i = 0; i < (EBaseEnum)EShaderPreprocessor::i_count; i++)
		m_bHasPreprocessor[i] = !uiVertexIndices[i].empty() || !uiGeometryIndices[i].empty() || !uiFragmentIndices[i].empty();

	CreateSubShaders( sVertexCode, sGeometryCode, sFragmentCode, uiVertexIndices, uiGeometryIndices, uiFragmentIndices, (EBaseEnum)0, 0, 1 );

	delete[] sVertexCode;
	if (sGeometryCode)
		delete[] sGeometryCode;

	delete[] sFragmentCode;

	m_bSuccess = true;
}

CShader::~CShader()
{
	for (std::pair<unsigned int, CSubShader *> itSubShaders : m_mapSubShaders)
		delete itSubShaders.second;
}

CSubShader *CShader::GetSubShader( EBaseEnum *eShaderPreprocessors ) const
{
	unsigned int uiKey = 0;
	unsigned int uiPreviousCount = 1;
	for (EBaseEnum i = 0; i < (EBaseEnum)EShaderPreprocessor::i_count; i++)
	{
		uiKey += uiPreviousCount * (m_bHasPreprocessor[i] ? (unsigned int)eShaderPreprocessors[i] : 0);
		uiPreviousCount *= (unsigned int)g_eShaderPreprocessorCount[i];
	}

	return m_mapSubShaders.find( uiKey )->second;
}

bool CShader::IsSuccess( void ) const
{
	return m_bSuccess;
}

void CShader::CreateSubShaders( char *sVertexCode, char *sGeometryCode, char *sFragmentCode, std::vector<unsigned int> *uiVertexIndices, std::vector<unsigned int> *uiGeometryIndices, std::vector<unsigned int> *uiFragmentIndices, EBaseEnum eIndex, unsigned int uiIndex, unsigned int uiPreviousCount )
{
	if (eIndex == (EBaseEnum)EShaderPreprocessor::i_count)
	{
		m_mapSubShaders.emplace( uiIndex, new CSubShader( sVertexCode, sGeometryCode, sFragmentCode ) );
		return;
	}

	for (EBaseEnum i = 0; i < (EBaseEnum)g_eShaderPreprocessorCount[eIndex]; i++)
	{
		if (m_bHasPreprocessor[eIndex])
		{
			if (!uiVertexIndices[eIndex].empty())
				sVertexCode[uiVertexIndices[eIndex][i]] = '1';

			if (!uiGeometryIndices[eIndex].empty())
				sGeometryCode[uiGeometryIndices[eIndex][i]] = '1';

			if (!uiFragmentIndices[eIndex].empty())
				sFragmentCode[uiFragmentIndices[eIndex][i]] = '1';
		}

		CreateSubShaders( sVertexCode, sGeometryCode, sFragmentCode, uiVertexIndices, uiGeometryIndices, uiFragmentIndices, eIndex + (EBaseEnum)1, uiIndex + (unsigned int)i * uiPreviousCount, uiPreviousCount * (unsigned int)g_eShaderPreprocessorCount[eIndex]);

		if (!m_bHasPreprocessor[eIndex])
			break;

		if (!uiVertexIndices[eIndex].empty())
			sVertexCode[uiVertexIndices[eIndex][i]] = '0';

		if (!uiGeometryIndices[eIndex].empty())
			sGeometryCode[uiGeometryIndices[eIndex][i]] = '0';

		if (!uiFragmentIndices[eIndex].empty())
			sFragmentCode[uiFragmentIndices[eIndex][i]] = '0';
	}
}

char *CShader::ReadShaderFile( const char *sPath, std::vector<unsigned int> *uiIndices )
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

				for (EBaseEnum i = 0; i < (EBaseEnum)EShaderPreprocessor::i_count; i++)
				{
					if (uiIndices[i].empty() && UTIL_streq( g_sShaderPreprocessorNames[i], sPreProcessorChar, UTIL_Min( sizeof( g_sShaderPreprocessorNames[i] ), iSubShaderSize ) - 1 ))
					{
						uiIndices[i].resize( g_eShaderPreprocessorCount[i]);

						for (EBaseEnum j = 0; j < g_eShaderPreprocessorCount[i]; j++)
						{
							const char *sSubShaderDefines = g_pShaderPreprocessorDefines[i][j];
							while (*sSubShaderDefines) cOutput.push_back( *sSubShaderDefines++ );
							uiIndices[i][j] = (unsigned int)cOutput.size() - 2;
						}
					}
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