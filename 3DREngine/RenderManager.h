#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "Global.h"
#include "CommandManager.h"

extern CConIVec2 cv_r_windowsize;
extern CConBool cb_r_fullscreen;
extern CConBool cb_r_vsync;
extern CConFloat cf_r_fov;
extern CConFloat cf_r_height;
extern CConFloat cf_r_near;
extern CConFloat cf_r_far;
extern CConInt ci_r_msaalevel;
extern CConFloat cf_r_vcsizefactor;
extern CConFloat cf_r_vcmsaalevelfactor;
extern CConString cs_r_windowname;

enum RenderPass_t : unsigned char
{
	RENDERPASS_SHADOW_DIR = 0,
	RENDERPASS_SHADOW_POINT,
	RENDERPASS_SHADOW_SPOT,
	RENDERPASS_SHADOW_CSM,
	RENDERPASS_DEPTH,
	RENDERPASS_UNLIT,
	RENDERPASS_LIT_DIR,
	RENDERPASS_LIT_POINT,
	RENDERPASS_LIT_SPOT,
	RENDERPASS_LIT_CSM,
	RENDERPASS_COUNT,
};

class CRenderManager
{
public:
	DECLARE_CLASS_NOBASE( CRenderManager );

	CRenderManager();
	~CRenderManager();

	void OnLoop( void );

	RenderPass_t GetRenderPass( void ) const;
	void SetRenderPass( RenderPass_t tRenderPass );

	GLFWmonitor *GetMonitor( void );
	GLFWwindow *GetWindow( void );

	void SetFrameBuffer( unsigned int uiFrameBuffer );
	void SetDepthFunc( unsigned int uiDepthFunc );
	void SetBlend( bool bBlend );
	void SetViewportSize( const glm::ivec2 &ivecViewportSize );
	void SetViewportOffset( const glm::ivec2 &ivecViewportOffset );

	int GetShadowMapIndex( void ) const;
	void SetShadowMapIndex( int iShadowMapIndex );

private:
	RenderPass_t m_tRenderPass;

	GLFWmonitor *m_pMonitor;
	GLFWwindow *m_pWindow;

	unsigned int m_uiFrameBuffer;
	unsigned int m_uiDepthFunc;
	bool m_bBlend;
	glm::ivec2 m_ivecViewportSize;
	glm::ivec2 m_ivecViewportOffset;

	int m_iShadowMapIndex;
};

#endif // RENDERMANAGER_H