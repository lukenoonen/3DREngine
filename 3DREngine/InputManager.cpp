#include "InputManager.h"
#include "RenderManager.h"
#include "TimeManager.h"
#include "CommandManager.h"

bool CC_Bind( const CTextLine *pCommand )
{
	const char *sKeyCode;
	if (!pCommand->GetValue( sKeyCode, 1 ))
		return false;

	EKeyCodes eKeyCode = UTIL_KeyStrToCode( sKeyCode );
	if (eKeyCode == EKeyCodes::i_invalid)
		return false;

	const char *sCommand;
	if (!pCommand->GetValue( sCommand, 2 ))
		return false;

	pInputManager->BindKey( eKeyCode, sCommand );

	return true;
}
CConCommand cc_bind( "bind", CC_Bind );

bool CC_Unbind( const CTextLine *pCommand )
{
	const char *sKeyCode;
	if (!pCommand->GetValue( sKeyCode, 1 ))
		return false;

	EKeyCodes eKeyCode = UTIL_KeyStrToCode( sKeyCode );
	if (eKeyCode == EKeyCodes::i_invalid)
		return false;

	pInputManager->UnbindKey( eKeyCode );

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
		pInputManager->SetKey( EKeyCodes::t_esc, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_DELETE:
		pInputManager->SetKey( EKeyCodes::t_del, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_BACKSPACE:
		pInputManager->SetKey( EKeyCodes::t_back, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_LEFT_SHIFT:
		pInputManager->SetKey( EKeyCodes::t_lsft, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_RIGHT_SHIFT:
		pInputManager->SetKey( EKeyCodes::t_rsft, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_SPACE:
		pInputManager->SetKey( EKeyCodes::t_spc, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_ENTER:
		pInputManager->SetKey( EKeyCodes::t_entr, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_TAB:
		pInputManager->SetKey( EKeyCodes::t_tab, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_LEFT_CONTROL:
		pInputManager->SetKey( EKeyCodes::t_lctl, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_RIGHT_CONTROL:
		pInputManager->SetKey( EKeyCodes::t_rctl, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_LEFT_ALT:
		pInputManager->SetKey( EKeyCodes::t_lalt, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_RIGHT_ALT:
		pInputManager->SetKey( EKeyCodes::t_ralt, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_GRAVE_ACCENT:
		pInputManager->SetKey( EKeyCodes::t_bq, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_RIGHT:
		pInputManager->SetKey( EKeyCodes::t_rght, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_LEFT:
		pInputManager->SetKey( EKeyCodes::t_left, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_DOWN:
		pInputManager->SetKey( EKeyCodes::t_down, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_UP:
		pInputManager->SetKey( EKeyCodes::t_up, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_0:
		pInputManager->SetKey( EKeyCodes::t_0, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_1:
		pInputManager->SetKey( EKeyCodes::t_1, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_2:
		pInputManager->SetKey( EKeyCodes::t_2, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_3:
		pInputManager->SetKey( EKeyCodes::t_3, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_4:
		pInputManager->SetKey( EKeyCodes::t_4, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_5:
		pInputManager->SetKey( EKeyCodes::t_5, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_6:
		pInputManager->SetKey( EKeyCodes::t_6, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_7:
		pInputManager->SetKey( EKeyCodes::t_7, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_8:
		pInputManager->SetKey( EKeyCodes::t_8, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_9:
		pInputManager->SetKey( EKeyCodes::t_9, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_A:
		pInputManager->SetKey( EKeyCodes::t_a, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_B:
		pInputManager->SetKey( EKeyCodes::t_b, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_C:
		pInputManager->SetKey( EKeyCodes::t_c, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_D:
		pInputManager->SetKey( EKeyCodes::t_d, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_E:
		pInputManager->SetKey( EKeyCodes::t_e, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_F:
		pInputManager->SetKey( EKeyCodes::t_f, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_G:
		pInputManager->SetKey( EKeyCodes::t_g, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_H:
		pInputManager->SetKey( EKeyCodes::t_h, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_I:
		pInputManager->SetKey( EKeyCodes::t_i, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_J:
		pInputManager->SetKey( EKeyCodes::t_j, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_K:
		pInputManager->SetKey( EKeyCodes::t_k, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_L:
		pInputManager->SetKey( EKeyCodes::t_l, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_M:
		pInputManager->SetKey( EKeyCodes::t_m, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_N:
		pInputManager->SetKey( EKeyCodes::t_n, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_O:
		pInputManager->SetKey( EKeyCodes::t_o, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_P:
		pInputManager->SetKey( EKeyCodes::t_p, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_Q:
		pInputManager->SetKey( EKeyCodes::t_q, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_R:
		pInputManager->SetKey( EKeyCodes::t_r, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_S:
		pInputManager->SetKey( EKeyCodes::t_s, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_T:
		pInputManager->SetKey( EKeyCodes::t_t, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_U:
		pInputManager->SetKey( EKeyCodes::t_u, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_V:
		pInputManager->SetKey( EKeyCodes::t_v, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_W:
		pInputManager->SetKey( EKeyCodes::t_w, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_X:
		pInputManager->SetKey( EKeyCodes::t_x, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_Y:
		pInputManager->SetKey( EKeyCodes::t_y, iAction == GLFW_PRESS );
		break;
	case GLFW_KEY_Z:
		pInputManager->SetKey( EKeyCodes::t_z, iAction == GLFW_PRESS );
		break;
	}
}

void CB_MouseButton( GLFWwindow *pWindow, int iButton, int iAction, int iMods )
{
	switch (iButton)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		pInputManager->SetKey( EKeyCodes::t_m0, iAction == GLFW_PRESS );
		break;
	case GLFW_MOUSE_BUTTON_RIGHT:
		pInputManager->SetKey( EKeyCodes::t_m1, iAction == GLFW_PRESS );
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		pInputManager->SetKey( EKeyCodes::t_m2, iAction == GLFW_PRESS );
		break;
	}
}

void CB_Scroll( GLFWwindow *pWindow, double dXOffset, double dYOffset )
{
	if (dYOffset != 0.0f)
	{
		if (dYOffset > 0.0f)
			pInputManager->SetKey( EKeyCodes::t_mwup, true );
		else
			pInputManager->SetKey( EKeyCodes::t_mwdn, true );
	}
}

void CB_CloseWindow( GLFWwindow *pWindow )
{
	pTimeManager->MarkForQuit();
}

const char *UTIL_KeyCodeToStr( EKeyCodes eKeyCode )
{
	return sKeycodeNames[(EBaseEnum)eKeyCode];
}

EKeyCodes UTIL_KeyStrToCode( const char *sKeyCode )
{
	for (EBaseEnum i = 0; i < (EBaseEnum)EKeyCodes::i_count; i++)
	{
		if (UTIL_streq( sKeyCode, sKeycodeNames[i] ))
			return (EKeyCodes)i;
	}

	return EKeyCodes::i_invalid;
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

	m_sCommand = UTIL_strdup( sCommand );
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

	m_vec2MousePosition = g_vec2Zero;
	m_vec2MouseDelta = g_vec2Zero;
}

void CInputManager::OnLoop( void )
{
	glfwPollEvents();

	GLFWwindow *pWindow = pRenderManager->GetWindow();
	double dXPos, dYPos;
	glfwGetCursorPos( pWindow, &dXPos, &dYPos );
	m_vec2MouseDelta = glm::vec2( (float)dXPos, (float)dYPos );
	if (m_vec2MouseDelta != g_vec2Zero)
	{
		m_vec2MousePosition += m_vec2MouseDelta;
		glfwSetCursorPos( pWindow, 0.0, 0.0 );
	}
}

const glm::vec2 &CInputManager::GetMousePosition( void )
{
	return m_vec2MousePosition;
}

const glm::vec2 &CInputManager::GetMouseDelta( void )
{
	return m_vec2MouseDelta;
}

void CInputManager::BindKey( EKeyCodes eKeyCode, const char *sCommand )
{
	m_KeyBinds[(EBaseEnum)eKeyCode].Bind( sCommand );
}

void CInputManager::UnbindKey( EKeyCodes eKeyCode )
{
	m_KeyBinds[(EBaseEnum)eKeyCode].Unbind();
}

void CInputManager::SetKey( EKeyCodes eKeyCode, bool bDown )
{
	if (bDown)
		m_KeyBinds[(EBaseEnum)eKeyCode].DownDispatch();
	else
		m_KeyBinds[(EBaseEnum)eKeyCode].UpDispatch();
}