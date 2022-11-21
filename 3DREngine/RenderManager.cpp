#include "RenderManager.h"
#include "InputManager.h"
#include "EntityManager.h"
#include "TimeManager.h"

bool CV_R_WindowSize( void );
CConIVec2 cv_r_windowsize( glm::ivec2( 800, 600 ), "r_windowsize", CV_R_WindowSize );
bool CV_R_WindowSize( void )
{
	const glm::ivec2 &vecWindowSize = cv_r_windowsize.GetValue();
	glfwSetWindowMonitor( pRenderManager->GetWindow(), cb_r_fullscreen.GetValue() ? pRenderManager->GetMonitor() : NULL, 0, 0, vecWindowSize.x, vecWindowSize.y, GLFW_DONT_CARE );
	return true;
}

bool CB_R_Fullscreen( void );
CConBool cb_r_fullscreen( false, "r_fullscreen", CB_R_Fullscreen );
bool CB_R_Fullscreen( void )
{
	const glm::ivec2 &vecWindowSize = cv_r_windowsize.GetValue();
	glfwSetWindowMonitor( pRenderManager->GetWindow(), cb_r_fullscreen.GetValue() ? pRenderManager->GetMonitor() : NULL, 0, 0, vecWindowSize.x, vecWindowSize.y, GLFW_DONT_CARE );
	return true;
}

bool CB_R_VSync( void );
CConBool cb_r_vsync( true, "r_vsync", CB_R_VSync );
bool CB_R_VSync( void )
{
	glfwSwapInterval( cb_r_vsync.GetValue() ? 1 : 0 );
	return true;
}

CConFloat cf_r_fov( 90.0f, "r_fov" );

CConFloat cf_r_height( 16.0f, "r_height" );

CConFloat cf_r_near( 0.1f, "r_near" );

CConFloat cf_r_far( 1000.0f, "r_far" );

CConInt ci_r_msaalevel( 4, "r_msaalevel" );

CConFloat cf_r_vcsizefactor( 1.0f, "r_vcsizefactor" );

CConFloat cf_r_vcmsaalevelfactor( 1.0f, "r_vcmsaalevelfactor" );

bool CB_R_WindowName( void );
CConString cs_r_windowname( "3DREngine", "r_windowname", CB_R_WindowName );
bool CB_R_WindowName( void )
{
	glfwSetWindowTitle( pRenderManager->GetWindow(), cs_r_windowname.GetValue() );
	return true;
}

bool CC_R_ShaderQuality( const CTextLine *pCommand )
{
	EBaseEnum eQuality;
	if (!pCommand->GetValue( eQuality, 1 ))
		return false;

	if (eQuality >= (EBaseEnum)EShaderPreprocessorQuality::i_count)
		return false;

	pRenderManager->SetShaderPreprocessor( EShaderPreprocessor::t_quality, eQuality );
	return true;
}
CConCommand cc_r_shaderquality( "r_shaderquality", CC_R_ShaderQuality );

CRenderManager::CRenderManager()
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

	m_eRenderPass = ERenderPass::i_invalid;

	m_pMonitor = glfwGetPrimaryMonitor();
	m_pWindow = glfwCreateWindow( 800, 600, "3DREngine", NULL, NULL );

	glfwMakeContextCurrent( m_pWindow );
	gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress );

	m_glFrameBuffer = 0;
	m_bBlend = false;
	m_ivec2ViewportSize = g_vec2Zero;
	m_ivec2ViewportOffset = g_vec2Zero;

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glBlendFunc( GL_ONE, GL_ONE );

	m_pActiveSubShader = NULL;

	for (EBaseEnum i = 0; i < (EBaseEnum)EShaderPreprocessor::i_count; i++)
		m_eShaderPreprocessors[i] = (EBaseEnum)0;

	for (EBaseEnum i = 0; i < (EBaseEnum)EShaderType::i_count; i++)
	{
		m_pShaders[i] = new CShader( g_sShaderTypeNames[i] );

		if (!m_pShaders[i]->Success())
		{
			// TODO: What should happen on failure?
		}
	}

	glGenBuffers( (EBaseEnum)EUniformBufferObjects::i_count, m_glUBOs );
	for (EBaseEnum i = 0; i < (EBaseEnum)EUniformBufferObjects::i_count; i++)
	{
		glBindBuffer( GL_UNIFORM_BUFFER, m_glUBOs[i] );
		glBufferData( GL_UNIFORM_BUFFER, g_glUBOSizes[i], NULL, GL_DYNAMIC_DRAW );
		glBindBufferRange( GL_UNIFORM_BUFFER, i, m_glUBOs[i], 0, g_glUBOSizes[i] );
	}

	GLint glTempMaxTextures;
	glGetIntegerv( GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &glTempMaxTextures );
	m_glMaxTextures = glTempMaxTextures - 1;

	glActiveTexture( GL_TEXTURE0 + m_glMaxTextures );

	m_glTextureIndex = 0;
}

CRenderManager::~CRenderManager()
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EShaderType::i_count; i++)
		delete m_pShaders[i];

	glDeleteBuffers( (EBaseEnum)EUniformBufferObjects::i_count, m_glUBOs );

	glfwDestroyWindow( m_pWindow );
	glfwTerminate();
}

void CRenderManager::OnLoop( void )
{
	glfwSwapBuffers( m_pWindow );
}

GLFWmonitor *CRenderManager::GetMonitor( void )
{
	return m_pMonitor;
}

GLFWwindow *CRenderManager::GetWindow( void )
{
	return m_pWindow;
}

void CRenderManager::SetFrameBuffer( GLuint glFrameBuffer )
{
	if (m_glFrameBuffer = glFrameBuffer)
	{
		m_glFrameBuffer = glFrameBuffer;
		glBindFramebuffer( GL_FRAMEBUFFER, glFrameBuffer );
		glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT );
	}
}

void CRenderManager::SetBlend( bool bBlend )
{
	if (m_bBlend != bBlend)
	{
		m_bBlend = bBlend;
		if (m_bBlend)
			glEnable( GL_BLEND );
		else
			glDisable( GL_BLEND );
	}
}

void CRenderManager::SetViewportSize( const glm::ivec2 &ivec2ViewportSize )
{
	if (m_ivec2ViewportSize != ivec2ViewportSize)
	{
		m_ivec2ViewportSize = ivec2ViewportSize;
		glViewport( m_ivec2ViewportOffset.x, m_ivec2ViewportOffset.y, ivec2ViewportSize.x, ivec2ViewportSize.y );
	}
}

void CRenderManager::SetViewportOffset( const glm::ivec2 &ivec2ViewportOffset )
{
	if (m_ivec2ViewportOffset != ivec2ViewportOffset)
	{
		m_ivec2ViewportOffset = ivec2ViewportOffset;
		glViewport( ivec2ViewportOffset.x, ivec2ViewportOffset.y, m_ivec2ViewportSize.x, m_ivec2ViewportSize.y );
	}
}

void CRenderManager::SetRenderPass( ERenderPass eRenderPass )
{
	m_eRenderPass = eRenderPass;
}

ERenderPass CRenderManager::GetRenderPass( void ) const
{
	return m_eRenderPass;
}

void CRenderManager::UseShader( EShaderType eShaderType )
{
	CSubShader *pNewActiveSubShader = m_pShaders[(EBaseEnum)eShaderType]->GetSubShader( m_eShaderPreprocessors );
	if (m_pActiveSubShader != pNewActiveSubShader)
	{
		m_pActiveSubShader = pNewActiveSubShader;
		m_pActiveSubShader->Use();
	}
}

void CRenderManager::SetUniformBufferObject( EUniformBufferObjects eBufferObject, unsigned int uiIndex, const void *pData )
{
	glBindBuffer( GL_UNIFORM_BUFFER, m_glUBOs[(EBaseEnum)eBufferObject] );
	glBufferSubData( GL_UNIFORM_BUFFER, g_pUBOParamOffsets[(EBaseEnum)eBufferObject][uiIndex], g_pUBOParamSizes[(EBaseEnum)eBufferObject][uiIndex], pData );
}

void CRenderManager::SetUniformBufferObject( EUniformBufferObjects eBufferObject, unsigned int uiIndex, unsigned int uiParamIndex, unsigned int uiParams, const void *pData )
{
	glBindBuffer( GL_UNIFORM_BUFFER, m_glUBOs[(EBaseEnum)eBufferObject] );
	glBufferSubData( GL_UNIFORM_BUFFER, g_pUBOParamOffsets[(EBaseEnum)eBufferObject][uiIndex] + (GLintptr)(g_pUBOParamSizes[(EBaseEnum)eBufferObject][uiIndex] * uiParamIndex), (GLsizeiptr)(g_pUBOParamSizes[(EBaseEnum)eBufferObject][uiIndex] * uiParams), pData );
}

void CRenderManager::SetUniform( const char *sName, bool bValue )
{
	glUniform1i( m_pActiveSubShader->GetLocation( sName ), (int)bValue );
}

void CRenderManager::SetUniform( const char *sName, int iValue )
{
	glUniform1i( m_pActiveSubShader->GetLocation( sName ), iValue );
}

void CRenderManager::SetUniform( const char *sName, float flValue )
{
	glUniform1f( m_pActiveSubShader->GetLocation( sName ), flValue );
}

void CRenderManager::SetUniform( const char *sName, const glm::vec2 &vec2Value )
{
	glUniform2fv( m_pActiveSubShader->GetLocation( sName ), 1, &vec2Value[0] );
}

void CRenderManager::SetUniform( const char *sName, float x, float y )
{
	glUniform2f( m_pActiveSubShader->GetLocation( sName ), x, y );
}

void CRenderManager::SetUniform( const char *sName, const glm::vec3 &vec3Value )
{
	glUniform3fv( m_pActiveSubShader->GetLocation( sName ), 1, &vec3Value[0] );
}

void CRenderManager::SetUniform( const char *sName, float x, float y, float z )
{
	glUniform3f( m_pActiveSubShader->GetLocation( sName ), x, y, z );
}

void CRenderManager::SetUniform( const char *sName, const glm::vec4 &vec4Value )
{
	glUniform4fv( m_pActiveSubShader->GetLocation( sName ), 1, &vec4Value[0] );
}

void CRenderManager::SetUniform( const char *sName, float x, float y, float z, float w )
{
	glUniform4f( m_pActiveSubShader->GetLocation( sName ), x, y, z, w );
}

void CRenderManager::SetUniform( const char *sName, const glm::mat2 &matValue )
{
	glUniformMatrix2fv( m_pActiveSubShader->GetLocation( sName ), 1, GL_FALSE, &matValue[0][0] );
}

void CRenderManager::SetUniform( const char *sName, const glm::mat3 &matValue )
{
	glUniformMatrix3fv( m_pActiveSubShader->GetLocation( sName ), 1, GL_FALSE, &matValue[0][0] );
}

void CRenderManager::SetUniform( const char *sName, const glm::mat4 &matValue )
{
	glUniformMatrix4fv( m_pActiveSubShader->GetLocation( sName ), 1, GL_FALSE, &matValue[0][0] );
}

void CRenderManager::SetShaderPreprocessor( EShaderPreprocessor eShaderPreprocessor, EBaseEnum eValue )
{
	m_eShaderPreprocessors[(EBaseEnum)eShaderPreprocessor] = eValue;
}

EBaseEnum CRenderManager::GetShaderPreprocessor( EShaderPreprocessor eShaderPreprocessor )
{
	return m_eShaderPreprocessors[(EBaseEnum)eShaderPreprocessor];
}

GLint CRenderManager::BindTexture( GLuint glTextureID, GLenum glTextureType )
{
	std::unordered_map<GLuint, GLint>::const_iterator itTextureIDSearch = m_mapTextureIDToIndex.find( glTextureID );
	if (itTextureIDSearch == m_mapTextureIDToIndex.end())
	{
		GLint glPrevTextureIndex = m_glTextureIndex;
		m_glTextureIndex = (m_glTextureIndex + 1) % m_glMaxTextures;

		std::unordered_map<GLint, GLuint>::const_iterator itIndexSearch = m_mapIndexToTextureID.find( glPrevTextureIndex );
		if (itIndexSearch != m_mapIndexToTextureID.end())
		{
			m_mapTextureIDToIndex.erase( itIndexSearch->second );
			m_mapIndexToTextureID.erase( itIndexSearch );
		}

		m_mapTextureIDToIndex.emplace( glTextureID, glPrevTextureIndex );
		m_mapIndexToTextureID.emplace( glPrevTextureIndex, glTextureID );

		glActiveTexture( GL_TEXTURE0 + glPrevTextureIndex );
		glBindTexture( glTextureType, glTextureID );
		glActiveTexture( GL_TEXTURE0 + m_glMaxTextures );

		return glPrevTextureIndex;
	}

	return itTextureIDSearch->second;
}

void CRenderManager::UnbindTexture( GLuint glTextureID )
{
	std::unordered_map<GLuint, GLint>::const_iterator itTextureIDSearch = m_mapTextureIDToIndex.find( glTextureID );
	if (itTextureIDSearch != m_mapTextureIDToIndex.end())
	{
		m_mapTextureIDToIndex.erase( itTextureIDSearch );
		m_mapIndexToTextureID.erase( itTextureIDSearch->second );
	}
}

void CRenderManager::UnbindAllTextures( void )
{
	m_mapTextureIDToIndex.clear();
	m_mapIndexToTextureID.clear();
	m_glTextureIndex = 0;
}