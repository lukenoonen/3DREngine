#include "RenderManager.h"
#include <stdlib.h>
#include "InputManager.h"
#include "ShaderManager.h"
#include "EntityManager.h"
#include "GlobalManager.h"

bool CV_R_WindowSize( void );
CConIVec2 cv_r_windowsize( glm::ivec2( 1920, 1080 ), "r_windowsize", CV_R_WindowSize );
bool CV_R_WindowSize( void )
{
	const glm::ivec2 &vecWindowSize = cv_r_windowsize.GetValue();
	glfwSetWindowMonitor( pRenderManager->GetWindow(), cb_r_fullscreen.GetValue() ? pRenderManager->GetMonitor() : NULL, 0, 0, vecWindowSize.x, vecWindowSize.y, GLFW_DONT_CARE );
	return true;
}

bool CB_R_Fullscreen( void );
CConBool cb_r_fullscreen( true, "r_fullscreen", CB_R_Fullscreen );
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

CConFloat cf_r_height( 1.0f, "r_height" );

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

CRenderManager::CRenderManager()
{
	glfwInit();
	glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
	glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
	glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
	glfwWindowHint( GLFW_DECORATED, GLFW_FALSE );
	glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

	m_tRenderPass = RENDERPASS_SHADOW_DIR;

	m_pMonitor = glfwGetPrimaryMonitor();
	m_pWindow = glfwCreateWindow( 800, 600, "3DREngine", NULL, NULL );

	glfwMakeContextCurrent( m_pWindow );
	gladLoadGLLoader( (GLADloadproc)glfwGetProcAddress );

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
}

CRenderManager::~CRenderManager()
{
	glfwDestroyWindow( m_pWindow );
	glfwTerminate();
}

void CRenderManager::OnLoop( void )
{
	for (unsigned int i = 0; i < (unsigned int)m_pCameraEntities.size(); i++)
	{
		if (m_pCameraEntities[i]->ShouldDraw())
			m_pCameraEntities[i]->Render();
	}

	glfwSwapBuffers( m_pWindow );
}

RenderPass_t CRenderManager::GetRenderPass( void ) const
{
	return m_tRenderPass;
}

void CRenderManager::SetRenderPass( RenderPass_t tRenderPass )
{
	m_tRenderPass = tRenderPass;
}

GLFWmonitor *CRenderManager::GetMonitor( void )
{
	return m_pMonitor;
}

GLFWwindow *CRenderManager::GetWindow( void )
{
	return m_pWindow;
}

void CRenderManager::DrawEntities( void )
{
	SetRenderPass( RENDERPASS_DEPTH );
	DrawNonLitEntities();

	SetRenderPass( RENDERPASS_UNLIT );
	DrawNonLitEntities();

	DrawLitEntities();
}

void CRenderManager::DrawNonLitEntities( void )
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

void CRenderManager::AddEntity( CBaseEntity *pEntity )
{
	if (pEntity->IsLight())
	{
		m_pLightEntities.push_back( dynamic_cast<CBaseLight *>(pEntity) );
	}
	else if (pEntity->IsCamera())
	{
		bool bSuccess = false;
		CBaseCamera *pCamera = dynamic_cast<CBaseCamera *>(pEntity);
		for (unsigned int i = 0; i < (unsigned int)m_pCameraEntities.size(); i++)
		{
			if (pCamera->GetRenderPriority() <= m_pCameraEntities[i]->GetRenderPriority())
			{
				m_pCameraEntities.insert( m_pCameraEntities.begin() + i, pCamera );
				bSuccess = true;
				break;
			}
		}

		if (!bSuccess)
			m_pCameraEntities.push_back( pCamera );
	}
	else if (pEntity->IsDrawable())
	{
		m_pDrawEntities.push_back( dynamic_cast<CBaseDrawable *>(pEntity) );
	}
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
	else if (pEntity->IsCamera())
	{
		for (unsigned int i = 0; i < m_pCameraEntities.size(); i++)
		{
			if (m_pCameraEntities[i] == pEntity)
			{
				m_pCameraEntities.erase( m_pCameraEntities.begin() + i );
				return;
			}
		}
	}
}

void CRenderManager::ClearEntities( void )
{
	m_pDrawEntities.clear();
	m_pLightEntities.clear();
	m_pCameraEntities.clear();
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

int CRenderManager::GetShadowMapIndex( void ) const
{
	return m_iShadowMapIndex;
}

void CRenderManager::SetShadowMapIndex( int iShadowMapIndex )
{
	m_iShadowMapIndex = iShadowMapIndex;
}