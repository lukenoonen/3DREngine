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

bool CC_ToggleCursor( void )
{
	pInputManager->ToggleCursor();

	return true;
}
CConCommand cc_togglecursor( "togglecursor", CC_ToggleCursor );

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

void CB_TextInput( GLFWwindow *pWindow, unsigned int uiChar )
{
	pInputManager->RecordText( uiChar, 0 );
}

void CB_TextInputKeyboard( GLFWwindow *pWindow, int iKey, int iScancode, int iAction, int iMods )
{
	if (iAction == GLFW_RELEASE)
		return;

	bool bRecordText = false;
	switch (iKey)
	{
	case GLFW_KEY_ESCAPE:
	case GLFW_KEY_DELETE:
	case GLFW_KEY_BACKSPACE:
	case GLFW_KEY_ENTER:
	case GLFW_KEY_TAB:
	case GLFW_KEY_RIGHT:
	case GLFW_KEY_LEFT:
	case GLFW_KEY_DOWN:
	case GLFW_KEY_UP:
		bRecordText = true;
		break;
	default:
		if (iMods & GLFW_MOD_CONTROL)
		{
			switch (iKey)
			{
			case GLFW_KEY_V:
				bRecordText = true;
				break;
			}
		}
		break;
	}

	if (bRecordText)
		pInputManager->RecordText( (unsigned int)iKey, iMods );
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
	glfwSetInputMode( pWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE );
	glfwSetKeyCallback( pWindow, CB_TextInputKeyboard );
	glfwSetCharCallback( pWindow, CB_TextInput );
	glfwSetMouseButtonCallback( pWindow, CB_MouseButton );
	glfwSetScrollCallback( pWindow, CB_Scroll );
	glfwSetWindowCloseCallback( pWindow, CB_CloseWindow );

	m_eCursorShape = ECursorShape::i_invalid;
	for (EBaseEnum i = 0; i < (EBaseEnum)ECursorShape::i_count; i++)
		m_pCursors[i] = glfwCreateStandardCursor( g_iCursorShapeValues[i] );
	SetCursor( ECursorShape::t_arrow );

	memset( m_bKeyDown, 0, sizeof( m_bKeyDown ) );
	memset( m_bPrevKeyDown, 0, sizeof( m_bPrevKeyDown ) );

	m_vec2CursorPosition = g_vec2Zero;
	m_vec2CursorPosition = -g_vec2One;
	m_vec2CursorDelta = g_vec2Zero;
	m_vec2CursorDelta = g_vec2Zero;

	m_bLockCursor = false;
	m_bLockKeyboard = false;

	LockKeyboard();
}

CInputManager::~CInputManager()
{
	for (EBaseEnum i = 0; i < (EBaseEnum)ECursorShape::i_count; i++)
		glfwDestroyCursor( m_pCursors[i] );
}

void CInputManager::OnLoop( void )
{
	m_sText.clear();
	memcpy( m_bPrevKeyDown, m_bKeyDown, sizeof( m_bPrevKeyDown ) );

	glfwPollEvents();

	GLFWwindow *pWindow = pRenderManager->GetWindow();
	double dXPos, dYPos;
	glfwGetCursorPos( pWindow, &dXPos, &dYPos );
	if (m_bLockCursor)
	{
		SetCursorDelta( glm::vec2( (float)dXPos, (float)dYPos ) );
		if (m_vec2CursorDelta != g_vec2Zero)
			glfwSetCursorPos( pWindow, 0.0, 0.0 );
	}
	else
	{
		glm::vec2 vec2NewCursorPosition = glm::vec2( (float)dXPos, (float)dYPos );
		SetCursorDelta( vec2NewCursorPosition - m_vec2CursorPosition );
		SetCursorPosition( vec2NewCursorPosition );
	}
}

const glm::vec2 &CInputManager::GetCursorPosition( void )
{
	return m_vec2CursorPosition;
}

const glm::vec2 &CInputManager::GetNormalizedCursorPosition( void )
{
	return m_vec2NormalizedCursorPosition;
}

const glm::vec2 &CInputManager::GetCursorDelta( void )
{
	return m_vec2CursorDelta;
}

const glm::vec2 &CInputManager::GetNormalizedCursorDelta( void )
{
	return m_vec2NormalizedCursorDelta;
}

void CInputManager::SetCursor( ECursorShape eCursorShape )
{
	if (m_eCursorShape != eCursorShape)
	{
		GLFWwindow *pWindow = pRenderManager->GetWindow();
		glfwSetCursor( pWindow, m_pCursors[(EBaseEnum)eCursorShape] );
		m_eCursorShape = eCursorShape;
	}
}

void CInputManager::ResetCursor( ECursorShape eCursorShape /* = ECursorShape::i_invalid */ )
{
	if (m_eCursorShape == eCursorShape || eCursorShape == ECursorShape::i_invalid)
		SetCursor( ECursorShape::t_arrow );
}

bool CInputManager::IsKeyPressed( EKeyCodes eKeyCode ) const
{
	return m_bKeyDown[(EBaseEnum)eKeyCode] && !m_bPrevKeyDown[(EBaseEnum)eKeyCode];
}

bool CInputManager::IsKeyDown( EKeyCodes eKeyCode ) const
{
	return m_bKeyDown[(EBaseEnum)eKeyCode];
}

bool CInputManager::IsKeyReleased( EKeyCodes eKeyCode ) const
{
	return !m_bKeyDown[(EBaseEnum)eKeyCode] && m_bPrevKeyDown[(EBaseEnum)eKeyCode];
}

void CInputManager::LockCursor( void )
{
	GLFWwindow *pWindow = pRenderManager->GetWindow();
	glfwSetInputMode( pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED );
	glfwSetCursorPos( pWindow, 0.0, 0.0 );
	m_bLockCursor = true;
}

void CInputManager::UnlockCursor( void )
{
	GLFWwindow *pWindow = pRenderManager->GetWindow();
	glfwSetInputMode( pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL );
	glfwSetCursorPos( pWindow, m_vec2CursorPosition.x, m_vec2CursorPosition.y );
	m_bLockCursor = false;
}

void CInputManager::ToggleCursor( void )
{
	if (m_bLockCursor)
		UnlockCursor();
	else
		LockCursor();
}

bool CInputManager::IsCursorLocked( void ) const
{
	return m_bLockCursor;
}

void CInputManager::LockKeyboard( void )
{
	GLFWwindow *pWindow = pRenderManager->GetWindow();
	glfwSetKeyCallback( pWindow, CB_KeyboardInput );
	glfwSetCharCallback( pWindow, NULL );
	m_bLockKeyboard = true;
}

void CInputManager::UnlockKeyboard( void )
{
	GLFWwindow *pWindow = pRenderManager->GetWindow();
	glfwSetKeyCallback( pWindow, CB_TextInputKeyboard );
	glfwSetCharCallback( pWindow, CB_TextInput );
	m_bLockKeyboard = false;
}

void CInputManager::ToggleKeyboard( void )
{
	if (m_bLockKeyboard)
		UnlockKeyboard();
	else
		LockKeyboard();
}

bool CInputManager::IsKeyboardLocked( void ) const
{
	return m_bLockKeyboard;
}

void CInputManager::BindKey( EKeyCodes eKeyCode, const char *sCommand )
{
	m_KeyBinds[(EBaseEnum)eKeyCode].Bind( sCommand );
}

void CInputManager::UnbindKey( EKeyCodes eKeyCode )
{
	m_KeyBinds[(EBaseEnum)eKeyCode].Unbind();
}

void CInputManager::RecordText( unsigned int uiChar, int iMods )
{
	m_sText.push_back( { uiChar, iMods } );
}

void CInputManager::SetKey( EKeyCodes eKeyCode, bool bDown )
{
	m_bKeyDown[(EBaseEnum)eKeyCode] = bDown;

	if (bDown)
		m_KeyBinds[(EBaseEnum)eKeyCode].DownDispatch();
	else
		m_KeyBinds[(EBaseEnum)eKeyCode].UpDispatch();
}

unsigned int CInputManager::GetTextCount( void ) const
{
	return (unsigned int)m_sText.size();
}

unsigned int CInputManager::GetTextChar( unsigned int uiIndex ) const
{
	return m_sText[uiIndex].uiChar;
}

int CInputManager::GetTextMods( unsigned int uiIndex ) const
{
	return m_sText[uiIndex].iMods;
}

const char *CInputManager::GetClipboardString( void ) const
{
	GLFWwindow *pWindow = pRenderManager->GetWindow();
	return glfwGetClipboardString( pWindow );
}

void CInputManager::SetClipboardString( const char *sString )
{
	GLFWwindow *pWindow = pRenderManager->GetWindow();
	return glfwSetClipboardString( pWindow, sString );
}

void CInputManager::SetCursorPosition( glm::vec2 vec2CursorPosition )
{
	m_vec2CursorPosition = vec2CursorPosition;
	m_vec2NormalizedCursorPosition = 2.0f * vec2CursorPosition / (glm::vec2)cv_r_windowsize.GetValue() - 1.0f;
	m_vec2NormalizedCursorPosition.y *= -1.0f;
}

void CInputManager::SetCursorDelta( glm::vec2 vec2CursorDelta )
{
	m_vec2CursorDelta = vec2CursorDelta;
	m_vec2NormalizedCursorDelta = 2.0f * vec2CursorDelta / (glm::vec2)cv_r_windowsize.GetValue();
	m_vec2NormalizedCursorDelta.y *= -1.0f;
}