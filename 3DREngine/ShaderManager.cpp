#include "ShaderManager.h"
#include "CommandManager.h"

bool CC_S_Quality( CTextItem *pCommand )
{
	if (pCommand->GetTextTermCount() < 2)
		return false;

	CTextTerm *pTextTerm = pCommand->GetTextTerm( 1 );
	if (!pTextTerm->IsUnsignedIntFormat())
		return false;

	unsigned char ucQuality = (unsigned char)pTextTerm->GetUnsignedInt();
	if (ucQuality < SHADERQUALITY_LOW || ucQuality >= SHADERQUALITY_COUNT)
		return false;

	pShaderManager->SetShaderQuality( (ShaderQuality_t)ucQuality );
	return true;
}
CConCommand cc_s_quality( "s_quality", CC_S_Quality );

CShaderManager::CShaderManager()
{
	m_pActiveSubShader = NULL;

	m_tShaderQuality = SHADERQUALITY_HIGH;
	m_tShaderAnimate = SHADERANIMATE_FALSE;
	m_tShaderShadow = SHADERSHADOW_FALSE;

	for (unsigned int i = 0; i < SHADERTYPE_COUNT; i++)
	{
		char *sVertexPath = UTIL_stradd( g_sShaderNames[i], ".vs" );
		char *sGeometryPath = UTIL_stradd( g_sShaderNames[i], ".gs" );
		char *sFragmentPath = UTIL_stradd( g_sShaderNames[i], ".fs" );
		m_pShaders[i] = new CShader( sVertexPath, sGeometryPath, sFragmentPath );
		delete[] sVertexPath;
		delete[] sGeometryPath;
		delete[] sFragmentPath;

		if (!m_pShaders[i]->IsSuccess())
		{
			// TODO: Abort!!
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
		delete m_pShaders[i];

	glDeleteBuffers( UBO_COUNT, m_uiUBOs );
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

void CShaderManager::SetShaderQuality( ShaderQuality_t tShaderQuality )
{
	m_tShaderQuality = tShaderQuality;
}

void CShaderManager::SetShaderAnimate( ShaderAnimate_t tShaderAnimate )
{
	m_tShaderAnimate = tShaderAnimate;
}

void CShaderManager::SetShaderShadow( ShaderShadow_t tShaderShadow )
{
	m_tShaderShadow = tShaderShadow;
}

void CShaderManager::Use( ShaderType_t tShaderType )
{
	m_pActiveSubShader = m_pShaders[tShaderType]->GetSubShader( m_tShaderQuality, m_tShaderAnimate, m_tShaderShadow );
	m_pActiveSubShader->Use();
}

void CShaderManager::SetValue( const char *sName, bool bValue )
{
	glUniform1i( m_pActiveSubShader->GetLocation( sName ), (int)bValue );
}

void CShaderManager::SetValue( const char *sName, int iValue )
{
	glUniform1i( m_pActiveSubShader->GetLocation( sName ), iValue );
}

void CShaderManager::SetValue( const char *sName, float flValue )
{
	glUniform1f( m_pActiveSubShader->GetLocation( sName ), flValue );
}

void CShaderManager::SetValue( const char *sName, const glm::vec2 &vecValue )
{
	glUniform2fv( m_pActiveSubShader->GetLocation( sName ), 1, &vecValue[0] );
}

void CShaderManager::SetValue( const char *sName, float x, float y )
{
	glUniform2f( m_pActiveSubShader->GetLocation( sName ), x, y );
}

void CShaderManager::SetValue( const char *sName, const glm::vec3 &vecValue )
{
	glUniform3fv( m_pActiveSubShader->GetLocation( sName ), 1, &vecValue[0] );
}

void CShaderManager::SetValue( const char *sName, float x, float y, float z )
{
	glUniform3f( m_pActiveSubShader->GetLocation( sName ), x, y, z );
}

void CShaderManager::SetValue( const char *sName, const glm::vec4 &vecValue )
{
	glUniform4fv( m_pActiveSubShader->GetLocation( sName ), 1, &vecValue[0] );
}

void CShaderManager::SetValue( const char *sName, float x, float y, float z, float w )
{
	glUniform4f( m_pActiveSubShader->GetLocation( sName ), x, y, z, w );
}

void CShaderManager::SetValue( const char *sName, const glm::mat2 &matValue )
{
	glUniformMatrix2fv( m_pActiveSubShader->GetLocation( sName ), 1, GL_FALSE, &matValue[0][0] );
}

void CShaderManager::SetValue( const char *sName, const glm::mat3 &matValue )
{
	glUniformMatrix3fv( m_pActiveSubShader->GetLocation( sName ), 1, GL_FALSE, &matValue[0][0] );
}

void CShaderManager::SetValue( const char *sName, const glm::mat4 &matValue )
{
	glUniformMatrix4fv( m_pActiveSubShader->GetLocation( sName ), 1, GL_FALSE, &matValue[0][0] );
}

ShaderQuality_t CShaderManager::GetShaderQuality( void ) const
{
	return m_tShaderQuality;
}

ShaderAnimate_t CShaderManager::GetShaderAnimate( void ) const
{
	return m_tShaderAnimate;
}

ShaderShadow_t CShaderManager::GetShaderShadow( void ) const
{
	return m_tShaderShadow;
}