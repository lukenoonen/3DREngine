#include "InputManager.h"
#include "RenderManager.h"
#include "GlobalManager.h"
#include "CommandManager.h"

bool CC_Bind( CTextItem *pCommand )
{
	if (pCommand->GetTextTermCount() < 3)
		return false;

	KeyCodes_t tKeyCode = UTIL_KeyStrToCode( pCommand->GetTextTerm( 1 )->GetString() );
	if (tKeyCode == KEYCODE_INVALID)
		return false;

	pInputManager->BindKey( tKeyCode, pCommand->GetTextTerm( 2 )->GetString() );

	return true;
}
CConCommand cc_bind( "bind", CC_Bind );

bool CC_Unbind( CTextItem *pCommand )
{
	if (pCommand->GetTextTermCount() < 2)
		return false;

	KeyCodes_t tKeyCodes = UTIL_KeyStrToCode( pCommand->GetTextTerm( 1 )->GetString() );
	if (tKeyCodes == KEYCODE_INVALID)
		return false;

	pInputManager->UnbindKey( tKeyCodes );

	return true;
}
CConCommand cc_unbind( "unbind", CC_Unbind );

void CB_KeyboardInput( GLFWwindow *pWindow, int iKey, int iScancode, int iAction, int iMods )
{
	if (iAction == GLFW_REPEAT)
		return;

	switch (iKey)
	{
	case GLFW_KEY_ESCAPE:
		pInputManager->SetKey( KEYCODE_ESCAPE, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_DELETE:
		pInputManager->SetKey( KEYCODE_DELETE, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_BACKSPACE:
		pInputManager->SetKey( KEYCODE_BACKSPACE, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_LEFT_SHIFT:
		pInputManager->SetKey( KEYCODE_LSHIFT, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		pInputManager->SetKey( KEYCODE_RSHIFT, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_SPACE:
		pInputManager->SetKey( KEYCODE_SPACE, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_ENTER:
		pInputManager->SetKey( KEYCODE_ENTER, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_TAB:
		pInputManager->SetKey( KEYCODE_TAB, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_LEFT_CONTROL:
		pInputManager->SetKey( KEYCODE_LCTL, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_RIGHT_CONTROL:
		pInputManager->SetKey( KEYCODE_RCTL, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_LEFT_ALT:
		pInputManager->SetKey( KEYCODE_LALT, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_RIGHT_ALT:
		pInputManager->SetKey( KEYCODE_RALT, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_GRAVE_ACCENT:
		pInputManager->SetKey( KEYCODE_BACKQUOTE, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_RIGHT:
		pInputManager->SetKey( KEYCODE_RIGHT, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_LEFT:
		pInputManager->SetKey( KEYCODE_LEFT, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_DOWN:
		pInputManager->SetKey( KEYCODE_DOWN, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_UP:
		pInputManager->SetKey( KEYCODE_UP, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_0:
		pInputManager->SetKey( KEYCODE_0, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_1:
		pInputManager->SetKey( KEYCODE_1, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_2:
		pInputManager->SetKey( KEYCODE_2, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_3:
		pInputManager->SetKey( KEYCODE_3, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_4:
		pInputManager->SetKey( KEYCODE_4, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_5:
		pInputManager->SetKey( KEYCODE_5, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_6:
		pInputManager->SetKey( KEYCODE_6, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_7:
		pInputManager->SetKey( KEYCODE_7, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_8:
		pInputManager->SetKey( KEYCODE_8, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_9:
		pInputManager->SetKey( KEYCODE_9, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_A:
		pInputManager->SetKey( KEYCODE_A, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_B:
		pInputManager->SetKey( KEYCODE_B, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_C:
		pInputManager->SetKey( KEYCODE_C, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_D:
		pInputManager->SetKey( KEYCODE_D, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_E:
		pInputManager->SetKey( KEYCODE_E, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_F:
		pInputManager->SetKey( KEYCODE_F, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_G:
		pInputManager->SetKey( KEYCODE_G, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_H:
		pInputManager->SetKey( KEYCODE_H, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_I:
		pInputManager->SetKey( KEYCODE_I, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_J:
		pInputManager->SetKey( KEYCODE_J, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_K:
		pInputManager->SetKey( KEYCODE_K, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_L:
		pInputManager->SetKey( KEYCODE_L, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_M:
		pInputManager->SetKey( KEYCODE_M, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_N:
		pInputManager->SetKey( KEYCODE_N, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_O:
		pInputManager->SetKey( KEYCODE_O, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_P:
		pInputManager->SetKey( KEYCODE_P, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_Q:
		pInputManager->SetKey( KEYCODE_Q, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_R:
		pInputManager->SetKey( KEYCODE_R, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_S:
		pInputManager->SetKey( KEYCODE_S, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_T:
		pInputManager->SetKey( KEYCODE_T, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_U:
		pInputManager->SetKey( KEYCODE_U, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_V:
		pInputManager->SetKey( KEYCODE_V, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_W:
		pInputManager->SetKey( KEYCODE_W, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_X:
		pInputManager->SetKey( KEYCODE_X, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_Y:
		pInputManager->SetKey( KEYCODE_Y, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_Z:
		pInputManager->SetKey( KEYCODE_Z, iAction == GLFW_PRESS );
		break;
	}
}

void CB_MouseButton( GLFWwindow *pWindow, int iButton, int iAction, int iMods )
{
	switch (iButton)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		pInputManager->SetKey( KEYCODE_LEFTMOUSE, iAction == GLFW_PRESS );
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		pInputManager->SetKey( KEYCODE_RIGHTMOUSE, iAction == GLFW_PRESS );
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		pInputManager->SetKey( KEYCODE_MIDDLEMOUSE, iAction == GLFW_PRESS );
		break;
	}
}

void CB_Scroll( GLFWwindow *pWindow, double dXOffset, double dYOffset )
{
	if (dYOffset != 0.0f)
	{
		if (dYOffset > 0.0f)
			pInputManager->SetKey( KEYCODE_MOUSEWHEELUP, true );
		else
			pInputManager->SetKey( KEYCODE_MOUSEWHEELDOWN, true );
	}
}

void CB_CloseWindow( GLFWwindow *pWindow )
{
	pGlobalValues->MarkForQuit();
}

const char *UTIL_KeyCodeToStr( KeyCodes_t tKeyCode )
{
	return sKeycodeNames[tKeyCode];
}

KeyCodes_t UTIL_KeyStrToCode( const char *sKeyCode )
{
	for (int i = 0; i < KEYCODE_COUNT; i++)
	{
		if (UTIL_strncmp( sKeyCode, sKeycodeNames[i] ) == 0)
			return (KeyCodes_t)i;
	}

	return KEYCODE_INVALID;
}

CKeyBind::CKeyBind()
{
	m_sCommand = NULL;
}

CKeyBind::~CKeyBind()
{
	if (m_sCommand)
		delete[] m_sCommand;
}

void CKeyBind::Bind( const char *sCommand )
{
	if (m_sCommand)
		delete[] m_sCommand;

	m_sCommand = UTIL_stredit( sCommand );
}

void CKeyBind::Unbind( void )
{
	if (m_sCommand)
	{
		delete[] m_sCommand;
		m_sCommand = NULL;
	}
}

void CKeyBind::DownDispatch( void )
{
	if (m_sCommand)
		pCommandManager->ProcessCommand( m_sCommand );
}

void CKeyBind::UpDispatch( void )
{
	if (m_sCommand && m_sCommand[0] == '+')
	{
		m_sCommand[0] = '-';
		pCommandManager->ProcessCommand( m_sCommand );
		m_sCommand[0] = '+';
	}
}

CInputManager::CInputManager()
{
	GLFWwindow *pWindow = pRenderManager->GetWindow();
	glfwSetInputMode( pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	glfwSetInputMode( pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE );
	glfwSetKeyCallback( pWindow, CB_KeyboardInput );
	glfwSetMouseButtonCallback( pWindow, CB_MouseButton );
	glfwSetScrollCallback( pWindow, CB_Scroll );
	glfwSetWindowCloseCallback( pWindow, CB_CloseWindow );
	glfwSetCursorPos( pWindow, 0, 0 );

	m_vecMousePosition = g_vec2Zero;
	m_vecMouseDelta = g_vec2Zero;
}

void CInputManager::OnLoop( void )
{
	glfwPollEvents();

	GLFWwindow *pWindow = pRenderManager->GetWindow();
	double dXPos, dYPos;
	glfwGetCursorPos( pWindow, &dXPos, &dYPos );
	m_vecMouseDelta = glm::vec2( (float)dXPos, (float)dYPos );
	if (m_vecMouseDelta != g_vec2Zero)
	{
		m_vecMousePosition += m_vecMouseDelta;
		glfwSetCursorPos( pWindow, 0.0, 0.0 );
	}
}

const glm::vec2 &CInputManager::GetMousePosition( void )
{
	return m_vecMousePosition;
}

const glm::vec2 &CInputManager::GetMouseDelta( void )
{
	return m_vecMouseDelta;
}

void CInputManager::BindKey( KeyCodes_t tKeyCode, const char *sCommand )
{
	m_KeyBinds[tKeyCode].Bind( sCommand );
}

void CInputManager::UnbindKey( KeyCodes_t tKeyCode )
{
	m_KeyBinds[tKeyCode].Unbind();
}

void CInputManager::SetKey( KeyCodes_t tKeyCode, bool bDown )
{
	if (bDown)
		m_KeyBinds[tKeyCode].DownDispatch();
	else
		m_KeyBinds[tKeyCode].UpDispatch();
}