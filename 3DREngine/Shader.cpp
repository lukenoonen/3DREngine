#include "Shader.h"
#include "UniformBufferObjects.h"
#include <iostream> // TODO: eliminate this
#include "FileManager.h"

CSubShader::CSubShader( GLsizei glProgramSize, GLenum glBinaryFormat, char *pData )
{
	m_bSuccess = false;

	m_glID = glCreateProgram();
	glProgramBinary( m_glID, glBinaryFormat, pData, glProgramSize );

	GLint glStatus;
	glGetProgramiv( m_glID, GL_LINK_STATUS, &glStatus );
	if (glStatus == GL_FALSE)
		return;

	Link();

	m_bSuccess = true;
}

CSubShader::CSubShader( const char *sVertexCode, const char *sGeometryCode, const char *sFragmentCode )
{
	m_bSuccess = false;

	m_glID = 0;

	GLuint glVertex, glGeometry, glFragment;

	glVertex = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( glVertex, 1, &sVertexCode, NULL );
	glCompileShader( glVertex );
	if (!CheckCompileErrorsShader( glVertex ))
		return;

	if (sGeometryCode)
	{
		glGeometry = glCreateShader( GL_GEOMETRY_SHADER );
		glShaderSource( glGeometry, 1, &sGeometryCode, NULL );
		glCompileShader( glGeometry );
		if (!CheckCompileErrorsShader( glGeometry ))
			return;
	}

	glFragment = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( glFragment, 1, &sFragmentCode, NULL );
	glCompileShader( glFragment );
	if (!CheckCompileErrorsShader( glFragment ))
		return;

	m_glID = glCreateProgram();
	glAttachShader( m_glID, glVertex );
	if (sGeometryCode)
		glAttachShader( m_glID, glGeometry );
	glAttachShader( m_glID, glFragment );

	glLinkProgram( m_glID );

	glDeleteShader( glVertex );
	if (sGeometryCode)
		glDeleteShader( glGeometry );

	glDeleteShader( glFragment );

	Link();

	m_bSuccess = true;
}

CSubShader::~CSubShader()
{
	for (unsigned int i = 0; i < m_sUniformNames.size(); i++)
		delete[] m_sUniformNames[i];

	if (m_glID != 0)
		glDeleteProgram( m_glID );
}

bool CSubShader::Success( void ) const
{
	return m_bSuccess;
}

void CSubShader::Use( void ) const
{
	glUseProgram( m_glID );
}

GLint CSubShader::GetID( void ) const
{
	return m_glID;
}

GLint CSubShader::GetLocation( const char *sName )
{
	std::unordered_map<const char *, GLint>::const_iterator itUniformSearch = m_mapUniformNameToLocation.find( sName );
	if (itUniformSearch == m_mapUniformNameToLocation.end())
	{
		for (unsigned int i = 0; i < m_sUniformNames.size(); i++)
		{
			if (UTIL_streq( sName, m_sUniformNames[i] ))
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

void CSubShader::Link( void )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EUniformBufferObjects::i_count; i++)
	{
		GLuint glIndex = glGetUniformBlockIndex( m_glID, g_sUniformBufferObjects[i] );
		if (glIndex != GL_INVALID_INDEX)
			glUniformBlockBinding( m_glID, glIndex, i );
	}

	GLint iUniformCount;
	GLint iMaxUniformLength;

	glGetProgramiv( m_glID, GL_ACTIVE_UNIFORMS, &iUniformCount );
	glGetProgramiv( m_glID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &iMaxUniformLength );

	for (GLint i = 0; i < iUniformCount; i++)
	{
		GLsizei iLength;
		GLint iSize;
		GLenum tType;
		GLchar *sUniformName = new GLchar[iMaxUniformLength];

		glGetActiveUniform( m_glID, i, iMaxUniformLength, &iLength, &iSize, &tType, sUniformName );
		GLint iLocation = glGetUniformLocation( m_glID, sUniformName );

		if (iLocation != -1)
		{
			m_iUniformLocations.push_back( iLocation );
			m_sUniformNames.push_back( sUniformName );
			if (iSize > 1)
			{
				GLchar *sUniformNameEdit = UTIL_strdup( sUniformName );
				*(UTIL_strchr( sUniformNameEdit, '[' ) + 1) = '\0';
				for (GLint j = 1; j < iSize; j++)
				{
					GLchar *sModifiedUniformName = new GLchar[iMaxUniformLength];
					sprintf_s( sModifiedUniformName, iMaxUniformLength, "%s%d]", sUniformNameEdit, j );
					m_iUniformLocations.push_back( glGetUniformLocation( m_glID, sModifiedUniformName ) );
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

bool CSubShader::CheckCompileErrorsShader( GLuint glShader )
{
	int iSuccess;
	glGetShaderiv( glShader, GL_COMPILE_STATUS, &iSuccess );
	if (!iSuccess)
	{
		char sInfoLog[1024];
		glGetShaderInfoLog( glShader, 1024, NULL, sInfoLog );
		std::cout << "ERROR::SHADER::COMPILATION_ERROR of type: SHADER\n" << sInfoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		return false;
	}

	return true;
}

CShader::CShader( const char *sShaderName, bool bCreateFromText )
{
	m_bSuccess = false;

	if (bCreateFromText)
	{
		if (!CreateShaderFromText( sShaderName ))
			return;
	}
	else
	{
		if (!CreateShaderFromBinary( sShaderName ))
		{
			DeleteSubShaders();

			if (!CreateShaderFromText( sShaderName ))
				return;
		}
	}

	m_bSuccess = true;
}

CShader::~CShader()
{
	DeleteSubShaders();
}

bool CShader::CreateShaderFromBinary( const char *sShaderName )
{
	if (!pFileManager->Open( sShaderName, EFileType::t_compiledshaderin ))
		return false;

	for (EBaseEnum i = 0; i < (EBaseEnum)EShaderPreprocessor::i_count; i++)
	{
		if (!pFileManager->Read( m_bHasPreprocessor[i] ))
			return false;
	}

	unsigned int uiSubShadersCount;
	if (!pFileManager->Read( uiSubShadersCount ))
		return false;

	for (unsigned int i = 0; i < uiSubShadersCount; i++)
	{
		unsigned int uiIndex;
		if (!pFileManager->Read( uiIndex ))
			return false;

		GLsizei glProgramSize;

		if (!pFileManager->Read( glProgramSize ))
			return false;

		GLenum glBinaryFormat;
		char *pData = new char[glProgramSize];

		if (!pFileManager->Read( glBinaryFormat ))
			return false;

		if (!pFileManager->ReadBytes( pData, glProgramSize ))
			return false;

		CSubShader *pSubShader = new CSubShader( glProgramSize, glBinaryFormat, pData );
		if (!pSubShader->Success())
			return false;

		m_mapSubShaders.emplace( uiIndex, pSubShader );
	}

	return true;
}

bool CShader::CreateShaderFromText( const char *sShaderName )
{
	char *sVertexCode;
	std::vector<unsigned int> uiVertexIndices[(EBaseEnum)EShaderPreprocessor::i_count];

	if (!pFileManager->Open( sShaderName, EFileType::t_vertexshader ) || !LoadShader( sVertexCode, uiVertexIndices ))
		return false;

	pFileManager->Close();

	char *sGeometryCode;
	std::vector<unsigned int> uiGeometryIndices[(EBaseEnum)EShaderPreprocessor::i_count];
	if (!pFileManager->Open( sShaderName, EFileType::t_geometryshader ) || !LoadShader( sGeometryCode, uiGeometryIndices ))
		sGeometryCode = NULL;
	else
		pFileManager->Close();

	char *sFragmentCode;
	std::vector<unsigned int> uiFragmentIndices[(EBaseEnum)EShaderPreprocessor::i_count];
	if (!pFileManager->Open( sShaderName, EFileType::t_fragmentshader ) || !LoadShader( sFragmentCode, uiFragmentIndices ))
	{
		delete[] sVertexCode;

		if (sGeometryCode)
			delete[] sGeometryCode;

		return false;
	}

	pFileManager->Close();

	for (EBaseEnum i = 0; i < (EBaseEnum)EShaderPreprocessor::i_count; i++)
		m_bHasPreprocessor[i] = !uiVertexIndices[i].empty() || !uiGeometryIndices[i].empty() || !uiFragmentIndices[i].empty();

	CreateSubShadersFromText( sVertexCode, sGeometryCode, sFragmentCode, uiVertexIndices, uiGeometryIndices, uiFragmentIndices, (EBaseEnum)0, 0, 1 );

	delete[] sVertexCode;
	if (sGeometryCode)
		delete[] sGeometryCode;

	delete[] sFragmentCode;

	if (!pFileManager->Open( sShaderName, EFileType::t_compiledshaderout ))
		return false;

	for (EBaseEnum i = 0; i < (EBaseEnum)EShaderPreprocessor::i_count; i++)
	{
		if (!pFileManager->Write( m_bHasPreprocessor[i] ))
			return false;
	}

	unsigned int uiSubShadersCount = (unsigned int)m_mapSubShaders.size();
	if (!pFileManager->Write( uiSubShadersCount ))
		return false;

	for (std::pair<unsigned int, CSubShader *> itSubShaders : m_mapSubShaders)
	{
		if (!pFileManager->Write( itSubShaders.first ))
			return false;
		
		GLsizei glProgramSize;
		glGetProgramiv( itSubShaders.second->GetID(), GL_PROGRAM_BINARY_LENGTH, &glProgramSize );

		if (!pFileManager->Write( glProgramSize ))
			return false;

		GLenum glBinaryFormat;
		char *pData = new char[glProgramSize];

		glGetProgramBinary( itSubShaders.second->GetID(), glProgramSize, NULL, &glBinaryFormat, pData );

		if (!pFileManager->Write( glBinaryFormat ))
			return false;

		if (!pFileManager->WriteBytes( pData, glProgramSize ))
			return false;
	}

	pFileManager->Close();

	return true;
}

bool CShader::Success( void ) const
{
	return m_bSuccess;
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

bool CShader::LoadShader( char *&sSource, std::vector<unsigned int> *uiIndices )
{
	char *sPreSource;

	if (!LoadShader( sPreSource ))
		return false;

	const char *sReadChar = sPreSource;

	std::vector<char> cSource;

	while (*sReadChar)
	{
		if (*sReadChar == '#')
		{
			const char *sPreProcessorChar = sReadChar + 1;
			int iPreProcessorSize = (int)(UTIL_strpbrk( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);
			if (UTIL_strneq( "subshader", sPreProcessorChar, UTIL_min( (int)sizeof( "subshader" ), iPreProcessorSize ) - 1 ))
			{
				sPreProcessorChar = UTIL_strskip( sPreProcessorChar + iPreProcessorSize, " \t\n" );
				int iSubShaderSize = (int)(UTIL_strpbrk( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);

				for (EBaseEnum i = 0; i < (EBaseEnum)EShaderPreprocessor::i_count; i++)
				{
					if (uiIndices[i].empty() && UTIL_strneq( g_sShaderPreprocessorNames[i], sPreProcessorChar, UTIL_min( (int)sizeof( g_sShaderPreprocessorNames[i] ), iSubShaderSize ) - 1 ))
					{
						uiIndices[i].resize( g_eShaderPreprocessorCount[i] );

						for (EBaseEnum j = 0; j < g_eShaderPreprocessorCount[i]; j++)
						{
							const char *sSubShaderDefines = g_pShaderPreprocessorDefines[i][j];
							while (*sSubShaderDefines) cSource.push_back( *sSubShaderDefines++ );
							uiIndices[i][j] = (unsigned int)(cSource.size() - 2);
						}
					}
				}

				sReadChar = sPreProcessorChar + iSubShaderSize + 1;
				continue;
			}
		}

		cSource.push_back( *sReadChar++ );
	}

	delete[] sPreSource;

	unsigned int uiSize = (unsigned int)cSource.size();
	sSource = new char[uiSize + 1];
	for (unsigned int i = 0; i < uiSize; i++)
		sSource[i] = cSource[i];

	sSource[uiSize] = '\0';
	return true;
}

bool CShader::LoadShader( char *&sSource )
{
	char *sContents;
	if (!pFileManager->Buffer( sContents ))
		return false;

	const char *sReadChar = sContents;

	std::vector<char> cSource;

	while (*sReadChar)
	{
		if (*sReadChar == '#')
		{
			const char *sPreProcessorChar = sReadChar + 1;
			int iPreProcessorSize = (int)(UTIL_strpbrk( sPreProcessorChar, " \t\n" ) - sPreProcessorChar);
			if (UTIL_strneq( "include", sPreProcessorChar, UTIL_min( (int)sizeof( "include" ), iPreProcessorSize ) ))
			{
				sPreProcessorChar = UTIL_strchr( sPreProcessorChar + iPreProcessorSize, '\"' ) + 1;
				int iPathSize = (int)(UTIL_strchr( sPreProcessorChar, '\"' ) - sPreProcessorChar);

				char *sPathName = UTIL_strndup( sPreProcessorChar, iPathSize );

				char *sIncludeSource;
				if (!pFileManager->Open( sPathName, EFileType::t_headershader ) || !LoadShader( sIncludeSource ))
				{
					delete[] sPathName;
					delete[] sContents;
					return false;
				}

				pFileManager->Close();

				const char *sIncludeSourceReadChar = sIncludeSource;
				while (*sIncludeSourceReadChar)
					cSource.push_back( *sIncludeSourceReadChar++ );

				delete[] sIncludeSource;

				sReadChar = sPreProcessorChar + iPathSize + 2;
				cSource.push_back( '\n' );

				delete[] sPathName;
				continue;
			}
		}

		cSource.push_back( *sReadChar++ );
	}

	delete[] sContents;

	unsigned int uiSize = (unsigned int)cSource.size();
	sSource = new char[uiSize + 1];
	for (unsigned int i = 0; i < uiSize; i++)
		sSource[i] = cSource[i];

	sSource[uiSize] = '\0';
	return true;
}

void CShader::CreateSubShadersFromText( char *sVertexCode, char *sGeometryCode, char *sFragmentCode, std::vector<unsigned int> *uiVertexIndices, std::vector<unsigned int> *uiGeometryIndices, std::vector<unsigned int> *uiFragmentIndices, EBaseEnum eIndex, unsigned int uiIndex, unsigned int uiPreviousCount )
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

		CreateSubShadersFromText( sVertexCode, sGeometryCode, sFragmentCode, uiVertexIndices, uiGeometryIndices, uiFragmentIndices, eIndex + (EBaseEnum)1, uiIndex + (unsigned int)i * uiPreviousCount, (unsigned int)g_eShaderPreprocessorCount[eIndex] * uiPreviousCount );

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

void CShader::DeleteSubShaders( void )
{
	for (std::pair<unsigned int, CSubShader *> itSubShaders : m_mapSubShaders)
		delete itSubShaders.second;
}