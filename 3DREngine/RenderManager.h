#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "Global.h"
#include "BaseDrawable.h"
#include "BaseLight.h"

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
	void DrawShadows( void );
	void DrawEntities( void );
	void DrawLitEntities( void );
	void DrawShadowMap( unsigned int uiDepthMapFBO );

	void AddEntity( CBaseEntity *pEntity );
	void RemoveEntity( CBaseEntity *pEntity );
	void ClearEntities( void );

	void SetWindowSize( const glm::ivec2 &ivecWindowSize );
	void SetFullscreen( bool bFullscreen );
	void SetVSync( bool bVSync );
	void SetMSAALevel( unsigned int uiMSAALevel );
	void SetWindowName( const char *sName );
	void SetFrameBuffer( unsigned int uiFrameBuffer );
	void SetDepthFunc( unsigned int uiDepthFunc );
	void SetBlend( bool bBlend );
	void SetViewportSize( const glm::ivec2 &ivecViewportSize );
	void SetViewportOffset( const glm::ivec2 &ivecViewportOffset );

	const glm::ivec2 &GetWindowSize( void );
	float GetWindowSizeRatio( void );

	GLFWwindow *GetWindow( void );

	void CreateShadowMapFramebuffer( unsigned int uiCount, unsigned int *uiShadowMapFBO, unsigned int *uiShadowMap, int iShadowSizeX, int iShadowSizeY );
	void DestroyShadowMapFrameBuffer( unsigned int uiCount, unsigned int *uiShadowMapFBO, unsigned int *uiShadowMap );
	int GetShadowMapIndex( unsigned iIndex ) const;
	void SetShadowMapIndex( int iShadowMapIndex, unsigned int iIndex );

	RenderPass_t GetRenderPass( void ) const;
	void SetRenderPass( RenderPass_t tRenderPass );

private:
	void CreateMSAABuffers( void );
	void DestroyMSAABuffers( void );

private:
	GLFWmonitor *m_pMonitor;
	GLFWwindow *m_pWindow;

	std::vector<CBaseLight *> m_pLightEntities;
	std::vector<CBaseDrawable *> m_pDrawEntities;

	glm::ivec2 m_ivecWindowSize;
	bool m_bFullscreen;
	bool m_bVSync;
	unsigned int m_uiMSAALevel;
	char *m_sWindowName;
	unsigned int m_uiFrameBuffer;
	unsigned int m_uiDepthFunc;
	bool m_bBlend;
	glm::ivec2 m_ivecViewportSize;
	glm::ivec2 m_ivecViewportOffset;

	int m_iShadowMapIndex[4];

	RenderPass_t m_tRenderPass;

	unsigned int m_uiFBO;
	unsigned m_uiTextureColorBufferMultiSampled;
	unsigned int m_uiRBO;
};

#endif // RENDERMANAGER_H