#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "Global.h"
#include "CommandManager.h"
#include "Shader.h"
#include "UniformBufferObjects.h"
#include "BaseFramebuffer.h"

extern CConIVec2 cv_r_windowsize;
extern CConBool cb_r_fullscreen;
extern CConBool cb_r_vsync;
extern CConFloat cf_r_fov;
extern CConFloat cf_r_width;
extern CConFloat cf_r_near;
extern CConFloat cf_r_far;
extern CConInt ci_r_msaalevel;
extern CConFloat cf_r_vcsizefactor;
extern CConFloat cf_r_vcmsaalevelfactor;
extern CConString cs_r_windowname;

enum class ERenderPass : EBaseEnum
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

	i_count,
	i_invalid = i_count,
};

class CRenderManager
{
public:
	DECLARE_CLASS_NOBASE( CRenderManager )

	CRenderManager();
	~CRenderManager();

	void RecompileShaders( void );

	void OnLoop( void );

	GLFWmonitor *GetMonitor( void );
	GLFWwindow *GetWindow( void );

	void SetFramebuffer( CBaseFramebuffer *pFramebuffer );
	void ClearFramebuffer( void );

	void SetBlend( bool bBlend );
	void SetFramebuffer( GLuint glFrameBuffer );
	void SetViewportSize( const glm::ivec2 &ivec2ViewportSize );
	void SetViewportOffset( const glm::ivec2 &ivec2ViewportOffset );

	void SetRenderPass( ERenderPass eRenderPass );

	ERenderPass GetRenderPass( void ) const;

	void UseShader( EShaderType eShaderType );

	void SetUniformBufferObject( EUniformBufferObjects eBufferObject, unsigned int uiIndex, const void *pData );
	void SetUniformBufferObject( EUniformBufferObjects eBufferObject, unsigned int uiIndex, unsigned int uiParamIndex, unsigned int uiParams, const void *pData );

	void SetUniform( const char *sName, bool bValue );
	void SetUniform( const char *sName, int iValue );
	void SetUniform( const char *sName, float flValue );
	void SetUniform( const char *sName, const glm::vec2 &vec2Value );
	void SetUniform( const char *sName, float x, float y );
	void SetUniform( const char *sName, const glm::vec3 &vec3Value );
	void SetUniform( const char *sName, float x, float y, float z );
	void SetUniform( const char *sName, const glm::vec4 &vec4Value );
	void SetUniform( const char *sName, float x, float y, float z, float w );
	void SetUniform( const char *sName, const glm::mat2 &matValue );
	void SetUniform( const char *sName, const glm::mat3 &matValue );
	void SetUniform( const char *sName, const glm::mat4 &matValue );

	void SetShaderPreprocessor( EShaderPreprocessor eShaderPreprocessor, EBaseEnum eValue );
	EBaseEnum GetShaderPreprocessor( EShaderPreprocessor eShaderPreprocessor );

	GLint BindTexture( GLuint glTextureID, GLenum glTextureType );
	void UnbindTexture( GLuint glTextureID );
	void UnbindAllTextures( void );

	CBaseFramebuffer *GetDefaultFramebuffer() const;

private:
	void CompileShaders( bool bCompileFromText );

private:
	ERenderPass m_eRenderPass;

	GLFWmonitor *m_pMonitor;
	GLFWwindow *m_pWindow;

	CBaseFramebuffer *m_pDefaultFramebuffer;
	CBaseFramebuffer *m_pActiveFramebuffer;

	bool m_bBlend;
	GLuint m_glFramebuffer;
	glm::ivec2 m_ivec2ViewportSize;
	glm::ivec2 m_ivec2ViewportOffset;

	GLuint m_glUBOs[(EBaseEnum)EUniformBufferObjects::i_count];

	CShader *m_pShaders[(EBaseEnum)EShaderType::i_count];
	CSubShader *m_pActiveSubShader;

	EBaseEnum m_eShaderPreprocessors[(EBaseEnum)EShaderPreprocessor::i_count];

	std::unordered_map<GLuint, GLint> m_mapTextureIDToIndex;
	std::unordered_map<GLint, GLuint> m_mapIndexToTextureID;
	GLint m_glTextureIndex;
	GLint m_glMaxTextures;
};

#endif // RENDERMANAGER_H