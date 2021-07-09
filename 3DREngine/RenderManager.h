#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "Global.h"
#include "BaseDrawable.h"
#include "BaseLight.h"
#include <SharedDefs.h>

class CRenderManager
{
public:
	DECLARE_CLASS_NOBASE( CRenderManager );

	CRenderManager();
	~CRenderManager();

	void AddEntity( CBaseEntity *pEntity );
	void RemoveEntity( CBaseEntity *pEntity );
	void ClearEntities( void );

	void OnLoop( void );
	void DrawShadows( void );
	void DrawEntities( void );
	void DrawLitEntities( void );
	unsigned int GetDrawFlags( void ) const;
	void SetDrawFlags( unsigned int uiDrawFlags );

	void SetWindowSize( const glm::ivec2 &ivecWindowSize );
	void SetFullscreen( bool bFullscreen );
	void SetVSync( bool bVSync );
	void SetMSAALevel( unsigned int uiMSAALevel );
	void SetWindowName( const char *sName );

	const glm::ivec2 &GetWindowSize( void );
	float GetWindowSizeRatio( void );

	GLFWwindow *GetWindow( void );

	void SetDepthFunc( unsigned int uiDepthFunc );
	void SetBlend( bool bBlend );
	void SetViewportSize( const glm::ivec2 &ivecViewportSize );
	void SetViewportOffset( const glm::ivec2 &ivecViewportOffset );

	void DrawShadowMap( unsigned int uiDepthMapFBO );

	int GetShadowMapIndex( unsigned iIndex ) const;
	void SetShadowMapIndex( int iShadowMapIndex, unsigned int iIndex );

private:
	void CreateMSAABuffers( void );
	void DestroyMSAABuffers( void );

private:
	GLFWwindow *m_pWindow;
	GLFWmonitor	*m_pMonitor;

	glm::ivec2 m_ivecWindowSize;
	bool m_bFullscreen;
	bool m_bVSync;
	unsigned int m_uiMSAALevel;
	char *m_sWindowName;

	unsigned int m_uiFBO;
	unsigned m_uiTextureColorBufferMultiSampled;
	unsigned int m_uiRBO;

	unsigned int m_uiDrawFlags;
	unsigned int m_uiDepthFunc;
	bool m_bBlend;
	glm::ivec2 m_ivecViewportSize;
	glm::ivec2 m_ivecViewportOffset;

	int m_iShadowMapIndex[4];

	std::vector<CBaseLight *> m_pLightEntities;
	std::vector<CBaseDrawable *> m_pDrawEntities;
};

#endif // RENDERMANAGER_H