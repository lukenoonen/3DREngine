#include "RenderManager.h"
#include <stdlib.h>
#include "InputManager.h"
#include "ShaderManager.h"
#include "EntityManager.h"
#include "CommandManager.h"

bool CC_R_WindowSize( CTextItem *pCommand )
{
	if (pCommand->GetTextTermCount() < 2)
		return false;;

	CTextTerm *pTextTerm = pCommand->GetTextTerm( 1 );
	if (!pTextTerm->IsIVec2Format())
		return false;
	
	pRenderManager->SetWindowSize( pTextTerm->GetIVec2() );
	return true;
}
CConCommand cc_r_windowsize( "r_windowsize", CC_R_WindowSize );

bool CC_R_Fullscreen( CTextItem *pCommand )
{
	if (pCommand->GetTextTermCount() < 2)
		return false;

	CTextTerm *pTextTerm = pCommand->GetTextTerm( 1 );
	if (!pTextTerm->IsBoolFormat())
		return false;

	pRenderManager->SetFullscreen( pTextTerm->GetBool() );
	return true;
}
CConCommand cc_r_fullscreen( "r_fullscreen", CC_R_Fullscreen );

bool CC_R_VSync( CTextItem *pCommand )
{
	if (pCommand->GetTextTermCount() < 2)
		return false;

	CTextTerm *pTextTerm = pCommand->GetTextTerm( 1 );
	if (!pTextTerm->IsBoolFormat())
		return false;

	pRenderManager->SetVSync( pTextTerm->GetBool() );
	return true;
}
CConCommand cc_r_vsync( "r_vsync", CC_R_VSync );

bool CC_R_MSAA( CTextItem *pCommand )
{
	if (pCommand->GetTextTermCount() < 2)
		return false;

	CTextTerm *pTextTerm = pCommand->GetTextTerm( 1 );
	if (!pTextTerm->IsUnsignedIntFormat())
		return false;

	pRenderManager->SetMSAALevel( pTextTerm->GetUnsignedInt() );
	return true;
}
CConCommand cc_r_msaa( "r_msaa", CC_R_MSAA );

bool CC_R_WindowName( CTextItem *pCommand )
{
	if (pCommand->GetTextTermCount() < 2)
		return false;

	const char *sWindowName = pCommand->GetTextTerm( 1 )->GetString();
	if (!sWindowName)
		return false;

	pRenderManager->SetWindowName( sWindowName );
	return true;
}
CConCommand cc_r_windowname( "r_windowname", CC_R_WindowName );

CRenderManager::CRenderManager()
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

	m_pMonitor = glfwGetPrimaryMonitor();
	m_pWindow = glfwCreateWindow( 800, 600, "3DREngine", NULL, NULL );

	glfwMakeContextCurrent( m_pWindow );
	gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress );

	m_ivecWindowSize = glm::ivec2( 800, 600 );
	m_bFullscreen = false;
	m_bVSync = true;
	m_uiMSAALevel = 0;
	m_sWindowName = NULL;
	m_uiFrameBuffer = 0;
	m_uiDepthFunc = GL_LEQUAL;
	m_bBlend = false;
	m_ivecViewportSize = g_vec2Zero;
	m_ivecViewportOffset = g_vec2Zero;

	glEnable( GL_DEPTH_TEST );
	glDepthFunc( GL_LEQUAL );
	glEnable( GL_CULL_FACE );
	glCullFace( GL_BACK );
	glBlendFunc( GL_ONE, GL_ONE );

	m_tRenderPass = RENDERPASS_SHADOW_DIR;
}

CRenderManager::~CRenderManager()
{
	if (m_sWindowName)
	{
		delete[] m_sWindowName;
		m_sWindowName = NULL;
	}

	if (m_uiMSAALevel != 0)
		DestroyMSAABuffers();

	glfwDestroyWindow( m_pWindow );
	glfwTerminate();
}

void CRenderManager::OnLoop( void )
{
	DrawShadows();

	SetViewportSize( m_ivecWindowSize );
	m_uiMSAALevel == 0 ? SetFrameBuffer( 0 ) : SetFrameBuffer( m_uiFBO );

	SetRenderPass( RENDERPASS_DEPTH );
	DrawEntities();

	SetRenderPass( RENDERPASS_UNLIT );
	DrawEntities();

	DrawLitEntities();

	if (m_uiMSAALevel != 0)
	{
		glBindFramebuffer( GL_READ_FRAMEBUFFER, m_uiFBO );
		glBindFramebuffer( GL_DRAW_FRAMEBUFFER, 0 );
		glBlitFramebuffer( 0, 0, m_ivecWindowSize.x, m_ivecWindowSize.y, 0, 0, m_ivecWindowSize.x, m_ivecWindowSize.y, GL_COLOR_BUFFER_BIT, GL_LINEAR );
	}	

	glfwSwapBuffers( m_pWindow );
}

void CRenderManager::DrawShadows( void )
{
	for (unsigned int i = 0; i < m_pLightEntities.size(); i++)
	{
		if (m_pLightEntities[i]->ShouldDraw())
			m_pLightEntities[i]->CalculateShadows();
	}
}

void CRenderManager::DrawEntities( void )
{
	for (unsigned int i = 0; i < m_pDrawEntities.size(); i++)
	{
		if (m_pDrawEntities[i]->ShouldDraw())
		{
			m_pDrawEntities[i]->PreDraw();
			m_pDrawEntities[i]->Draw();
		}
	}
}

void CRenderManager::DrawLitEntities( void )
{
	for (unsigned int i = 0; i < m_pDrawEntities.size(); i++)
	{
		if (m_pDrawEntities[i]->ShouldDraw())
		{
			SetBlend( false );
			m_pDrawEntities[i]->PreDraw();
			unsigned int uiDrawCount = 0;
			for (unsigned int j = 0; j < m_pLightEntities.size(); j++)
			{
				if (m_pLightEntities[j]->ShouldDraw())
				{
					if (uiDrawCount == 1)
						SetBlend( true );

					m_pLightEntities[j]->ActivateLight();
					m_pDrawEntities[i]->Draw();
					uiDrawCount++;
				}
			}
		}
	}

	SetBlend( false );
}

void CRenderManager::DrawShadowMap( unsigned int uiDepthMapFBO )
{
	SetFrameBuffer( uiDepthMapFBO );
	DrawEntities();
}

void CRenderManager::AddEntity( CBaseEntity *pEntity )
{
	if (pEntity->IsLight())
		m_pLightEntities.push_back( dynamic_cast<CBaseLight *>(pEntity) );
	else if (pEntity->IsDrawable())
		m_pDrawEntities.push_back( dynamic_cast<CBaseDrawable *>(pEntity) );
}

void CRenderManager::RemoveEntity( CBaseEntity *pEntity )
{
	if (pEntity->IsLight())
	{
		for (unsigned int i = 0; i < m_pLightEntities.size(); i++)
		{
			if (m_pLightEntities[i] == pEntity)
			{
				m_pLightEntities.erase( m_pLightEntities.begin() + i );
				return;
			}
		}
	}
	else if (pEntity->IsDrawable())
	{
		for (unsigned int i = 0; i < m_pDrawEntities.size(); i++)
		{
			if (m_pDrawEntities[i] == pEntity)
			{
				m_pDrawEntities.erase( m_pDrawEntities.begin() + i );
				return;
			}
		}
	}
}

void CRenderManager::ClearEntities( void )
{
	m_pDrawEntities.clear();
	m_pLightEntities.clear();
}

void CRenderManager::SetWindowSize( const glm::ivec2 &ivecWindowSize )
{
	if (m_ivecWindowSize == ivecWindowSize)
		return;

	m_ivecWindowSize = ivecWindowSize;
	glfwSetWindowMonitor( m_pWindow, m_bFullscreen ? m_pMonitor : NULL, 0, 0, m_ivecWindowSize.x, m_ivecWindowSize.y, GLFW_DONT_CARE );

	if (m_uiMSAALevel != 0)
	{
		DestroyMSAABuffers();
		CreateMSAABuffers();
	}
}

void CRenderManager::SetFullscreen( bool bFullscreen )
{
	if (m_bFullscreen == bFullscreen)
		return;

	m_bFullscreen = bFullscreen;
	glfwSetWindowMonitor( m_pWindow, m_bFullscreen ? m_pMonitor : NULL, 0, 0, m_ivecWindowSize.x, m_ivecWindowSize.y, GLFW_DONT_CARE );
}

void CRenderManager::SetVSync( bool bVSync )
{
	if (m_bVSync == bVSync)
		return;

	m_bVSync = bVSync;
	glfwSwapInterval( m_bVSync ? 1 : 0 );
}

void CRenderManager::SetMSAALevel( unsigned int uiMSAALevel )
{
	if (m_uiMSAALevel == uiMSAALevel)
		return;

	m_uiMSAALevel = uiMSAALevel;
	if (m_uiMSAALevel != 0)
	{
		DestroyMSAABuffers();
		CreateMSAABuffers();
	}
	else
	{
		DestroyMSAABuffers();
	}
}

void CRenderManager::SetWindowName( const char *sWindowName )
{
	if (m_sWindowName)
		delete[] m_sWindowName;

	m_sWindowName = UTIL_stredit( sWindowName );
	glfwSetWindowTitle( m_pWindow, m_sWindowName );
}

void CRenderManager::SetFrameBuffer( unsigned int uiFrameBuffer )
{
	if (m_uiFrameBuffer = uiFrameBuffer)
	{
		m_uiFrameBuffer = uiFrameBuffer;
		glBindFramebuffer( GL_FRAMEBUFFER, uiFrameBuffer );
		glClear( GL_DEPTH_BUFFER_BIT );
	}
}

void CRenderManager::SetDepthFunc( unsigned int uiDepthFunc )
{
	if (m_uiDepthFunc != uiDepthFunc)
	{
		m_uiDepthFunc = uiDepthFunc;
		glDepthFunc( uiDepthFunc );
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

void CRenderManager::SetViewportSize( const glm::ivec2 &ivecViewportSize )
{
	if (m_ivecViewportSize != ivecViewportSize)
	{
		m_ivecViewportSize = ivecViewportSize;
		glViewport( m_ivecViewportOffset.x, m_ivecViewportOffset.y, ivecViewportSize.x, ivecViewportSize.y );
	}
}

void CRenderManager::SetViewportOffset( const glm::ivec2 &ivecViewportOffset )
{
	if (m_ivecViewportOffset != ivecViewportOffset)
	{
		m_ivecViewportOffset = ivecViewportOffset;
		glViewport( ivecViewportOffset.x, ivecViewportOffset.y, m_ivecViewportSize.x, m_ivecViewportSize.y );
	}
}

const glm::ivec2 &CRenderManager::GetWindowSize( void )
{
	return m_ivecWindowSize;
}

float CRenderManager::GetWindowSizeRatio( void )
{
	return (float)m_ivecWindowSize.x / (float)m_ivecWindowSize.y;
}

GLFWwindow *CRenderManager::GetWindow( void )
{
	return m_pWindow;
}

void CRenderManager::CreateShadowMapFramebuffer( unsigned int uiCount, unsigned int *uiShadowMapFBO, unsigned int *uiShadowMap, int iShadowSizeX, int iShadowSizeY )
{
	glGenFramebuffers( uiCount, uiShadowMapFBO );
	glGenTextures( uiCount, uiShadowMap );

	for (unsigned int i = 0; i < uiCount; i++)
	{
		glBindTexture( GL_TEXTURE_2D, uiShadowMap[i] );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, iShadowSizeX, iShadowSizeY, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE );
		glBindFramebuffer( GL_FRAMEBUFFER, uiShadowMapFBO[i] );
		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, uiShadowMap[i], 0 );
		glDrawBuffer( GL_NONE );
		glReadBuffer( GL_NONE );
	}
}

void CRenderManager::DestroyShadowMapFrameBuffer( unsigned int uiCount, unsigned int *uiShadowMapFBO, unsigned int *uiShadowMap )
{
	glDeleteFramebuffers( uiCount, uiShadowMapFBO );
	glDeleteTextures( uiCount, uiShadowMap );
}

int CRenderManager::GetShadowMapIndex( unsigned iIndex ) const
{
	return m_iShadowMapIndex[iIndex];
}

void CRenderManager::SetShadowMapIndex( int iShadowMapIndex, unsigned int iIndex )
{
	m_iShadowMapIndex[iIndex] = iShadowMapIndex;
}

RenderPass_t CRenderManager::GetRenderPass( void ) const
{
	return m_tRenderPass;
}

void CRenderManager::SetRenderPass( RenderPass_t tRenderPass )
{
	m_tRenderPass = tRenderPass;
}

void CRenderManager::CreateMSAABuffers( void )
{
	glGenFramebuffers( 1, &m_uiFBO );
	glGenTextures( 1, &m_uiTextureColorBufferMultiSampled );
	glGenRenderbuffers( 1, &m_uiRBO );

	glBindFramebuffer( GL_FRAMEBUFFER, m_uiFBO );
	glBindTexture( GL_TEXTURE_2D_MULTISAMPLE, m_uiTextureColorBufferMultiSampled );
	glBindRenderbuffer( GL_RENDERBUFFER, m_uiRBO );

	glTexImage2DMultisample( GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB8, m_ivecWindowSize.x, m_ivecWindowSize.y, GL_TRUE );
	glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_uiTextureColorBufferMultiSampled, 0 );
	glRenderbufferStorageMultisample( GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, m_ivecWindowSize.x, m_ivecWindowSize.y );
	glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_uiRBO );
}

void CRenderManager::DestroyMSAABuffers( void )
{
	glDeleteFramebuffers( 1, &m_uiFBO );
	glDeleteTextures( 1, &m_uiTextureColorBufferMultiSampled );
	glDeleteRenderbuffers( 1, &m_uiRBO );
}