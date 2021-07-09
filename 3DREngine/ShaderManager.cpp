#include "ShaderManager.h"
#include <UTIL.h>
#include <stdio.h>

bool CC_S_Quality( const std::vector<const char *> &sCommands )
{
	if (sCommands.size() < 1)
		return false;

	int iQuality = UTIL_atoi( sCommands[0] );
	if (iQuality <= (int)SHADERQUALITY_DEFAULT || iQuality >= (int)SHADERQUALITY_COUNT)
		return false;

	pShaderManager->SetShaderQuality( (ShaderQuality_t)iQuality );
	return true;
}
CConCommand cc_s_quality( "s_quality", CC_S_Quality );

CShaderManager::CShaderManager()
{
	m_pActiveShader = NULL;

	m_tOverrideShaderType = SHADERTYPE_INVALID;
	m_tShaderSubType = SHADERSUBTYPE_DEFAULT;
	m_tShaderQuality = SHADERQUALITY_DEFAULT;

	for (unsigned int i = 0; i < SHADERTYPE_COUNT; i++)
	{
		char sVertexShader[260];
		char sGeometryShader[260];
		char sFragmentShader[260];

		sprintf_s( sVertexShader, sizeof( sVertexShader ), "resources/shaders/%s.vs", g_sShaderNames[i] );
		sprintf_s( sGeometryShader, sizeof( sGeometryShader ), "resources/shaders/%s.gs", g_sShaderNames[i] );
		sprintf_s( sFragmentShader, sizeof( sFragmentShader ), "resources/shaders/%s.fs", g_sShaderNames[i] );

		for (unsigned int j = 0; j < SHADERSUBTYPE_COUNT; j++)
		{
			for (unsigned int k = 0; k < SHADERQUALITY_COUNT; k++)
			{
				m_pShaders[i][j][k] = new CShader( sVertexShader, sGeometryShader, sFragmentShader, (ShaderSubType_t)j, (ShaderQuality_t)k );
				if (!m_pShaders[i][j][k]->IsSuccess())
				{
					delete m_pShaders[i][j][k];
					m_pShaders[i][j][k] = NULL;
				}
			}
		}
	}

	glGenBuffers( UBO_COUNT, m_uiUBOs );
	for (unsigned int i = 0; i < UBO_COUNT; i++)
	{
		glBindBuffer( GL_UNIFORM_BUFFER, m_uiUBOs[i] );
		glBufferData( GL_UNIFORM_BUFFER, g_uiUBOSizes[i], NULL, GL_DYNAMIC_DRAW );
		glBindBufferRange( GL_UNIFORM_BUFFER, i, m_uiUBOs[i], 0, g_uiUBOSizes[i] );
	}
}

CShaderManager::~CShaderManager()
{
	for (unsigned int i = 0; i < SHADERTYPE_COUNT; i++)
	{
		for (unsigned int j = 0; j < SHADERSUBTYPE_COUNT; j++)
		{
			for (unsigned int k = 0; k < SHADERQUALITY_COUNT; k++)
			{
				if (m_pShaders[i][j][k])
					delete m_pShaders[i][j][k];
			}
		}
	}

	glDeleteBuffers( UBO_COUNT, m_uiUBOs );
}

CShader *CShaderManager::GetShader( ShaderType_t tShaderType ) const
{
	if (m_pShaders[tShaderType][m_tShaderSubType][m_tShaderQuality])
		return m_pShaders[tShaderType][m_tShaderSubType][m_tShaderQuality];

	return m_pShaders[tShaderType][m_tShaderSubType][SHADERQUALITY_DEFAULT];
}

void CShaderManager::SetUniformBufferObject( UniformBufferObjects_t tBufferObject, unsigned int uiIndex, const void *pData )
{
	glBindBuffer( GL_UNIFORM_BUFFER, m_uiUBOs[tBufferObject] );
    glBufferSubData( GL_UNIFORM_BUFFER, g_pUBOParamOffsets[tBufferObject][uiIndex], g_pUBOParamSizes[tBufferObject][uiIndex], pData );
}

void CShaderManager::SetUniformBufferObject( UniformBufferObjects_t tBufferObject, unsigned int uiIndex, unsigned int uiParamIndex, unsigned int uiParams, const void *pData )
{
	glBindBuffer( GL_UNIFORM_BUFFER, m_uiUBOs[tBufferObject] );
	glBufferSubData( GL_UNIFORM_BUFFER, g_pUBOParamOffsets[tBufferObject][uiIndex] + g_pUBOParamSizes[tBufferObject][uiIndex] * uiParamIndex, g_pUBOParamSizes[tBufferObject][uiIndex] * uiParams, pData );
}

void CShaderManager::SetShaderSubType( ShaderSubType_t tShaderSubType )
{
	m_tShaderSubType = tShaderSubType;
}

ShaderSubType_t CShaderManager::GetShaderSubType( void ) const
{
	return m_tShaderSubType;
}

void CShaderManager::SetShaderQuality( ShaderQuality_t tShaderQuality )
{
	m_tShaderQuality = tShaderQuality;
}

ShaderQuality_t CShaderManager::GetShaderQuality( void ) const
{
	return m_tShaderQuality;
}

void CShaderManager::SetOverrideShader( ShaderType_t tShaderType )
{
	m_tOverrideShaderType = tShaderType;
}

CShader *CShaderManager::GetOverrideShader( void ) const
{
	if (m_tOverrideShaderType != SHADERTYPE_INVALID)
		return GetShader( m_tOverrideShaderType );

	return NULL;
}

void CShaderManager::SetActiveShader( CShader *pShader )
{
	m_pActiveShader = pShader;
}

CShader *CShaderManager::GetActiveShader( void ) const
{
	return m_pActiveShader;
}