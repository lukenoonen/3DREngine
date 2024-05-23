#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include "Global.h"

enum class EKeyCodes : EBaseEnum
{
	t_m0,
	t_m1,
	t_m2,
	t_mwup,
	t_mwdn,
	t_esc,
	t_del,
	t_back,
	t_lsft,
	t_rsft,
	t_spc,
	t_entr,
	t_tab,
	t_lctl,
	t_rctl,
	t_lalt,
	t_ralt,
	t_bq,
	t_rght,
	t_left,
	t_down,
	t_up,
	t_0,
	t_1,
	t_2,
	t_3,
	t_4,
	t_5,
	t_6,
	t_7,
	t_8,
	t_9,
	t_a,
	t_b,
	t_c,
	t_d,
	t_e,
	t_f,
	t_g,
	t_h,
	t_i,
	t_j,
	t_k,
	t_l,
	t_m,
	t_n,
	t_o,
	t_p,
	t_q,
	t_r,
	t_s,
	t_t,
	t_u,
	t_v,
	t_w,
	t_x,
	t_y,
	t_z,

	i_count,
	i_invalid = i_count,
};

static const char *sKeycodeNames[] =
{
	"m0",
	"m1",
	"m2",
	"mwup",
	"mwdn",
	"esc",
	"del",
	"back",
	"lsft",
	"rsft",
	"spc",
	"entr",
	"tab",
	"lctl",
	"rctl",
	"lalt",
	"ralt",
	"bq",
	"rght",
	"left",
	"down",
	"up",
	"0",
	"1",
	"2",
	"3",
	"4",
	"5",
	"6",
	"7",
	"8",
	"9",
	"a",
	"b",
	"c",
	"d",
	"e",
	"f",
	"g",
	"h",
	"i",
	"j",
	"k",
	"l",
	"m",
	"n",
	"o",
	"p",
	"q",
	"r",
	"s",
	"t",
	"u",
	"v",
	"w",
	"x",
	"y",
	"z"
};

const char *UTIL_KeyCodeToStr( EKeyCodes eKeyCode );
EKeyCodes UTIL_KeyStrToCode( const char *sKeyCode );

class CKeyBind
{
public:
	DECLARE_CLASS_NOBASE( CKeyBind )

	CKeyBind();
	~CKeyBind();

	void Bind( const char *sCommand );
	void Unbind( void );

	void DownDispatch( void );
	void UpDispatch( void );

private:
	char *m_sCommand;
};

enum class ECursorShape : EBaseEnum
{
	t_arrow,
	t_ibeam,
	t_crosshair,
	t_hand,
	t_hresize,
	t_vresize,

	i_count,
	i_invalid = i_count,
};

static int g_iCursorShapeValues[] =
{
	GLFW_ARROW_CURSOR,
	GLFW_IBEAM_CURSOR,
	GLFW_CROSSHAIR_CURSOR,
	GLFW_HAND_CURSOR,
	GLFW_HRESIZE_CURSOR,
	GLFW_VRESIZE_CURSOR,
};

struct SInputChar
{
	unsigned int uiChar;
	int iMods;
};

class CInputManager
{
public:
	DECLARE_CLASS_NOBASE( CInputManager )

	CInputManager();
	~CInputManager();

	void OnLoop( void );

	const glm::vec2 &GetCursorPosition( void );
	const glm::vec2 &GetNormalizedCursorPosition( void );

	const glm::vec2 &GetCursorDelta( void );
	const glm::vec2 &GetNormalizedCursorDelta( void );

	void SetCursor( ECursorShape eCursorShape );
	void ResetCursor( ECursorShape eCursorShape = ECursorShape::i_invalid );

	bool IsKeyPressed( EKeyCodes eKeyCode ) const;
	bool IsKeyDown( EKeyCodes eKeyCode ) const;
	bool IsKeyReleased( EKeyCodes eKeyCode ) const;

	void LockCursor( void );
	void UnlockCursor( void );
	void ToggleCursor( void );
	bool IsCursorLocked( void ) const;

	void LockKeyboard( void );
	void UnlockKeyboard( void );
	void ToggleKeyboard( void );
	bool IsKeyboardLocked( void ) const;

	void BindKey( EKeyCodes eKeyCode, const char *sCommand );
	void UnbindKey( EKeyCodes eKeyCode );

	void RecordText( unsigned int uiChar, int iMods );
	void SetKey( EKeyCodes eKeyCode, bool bDown );

	unsigned int GetTextCount( void ) const;
	unsigned int GetTextChar( unsigned int uiIndex ) const;
	int GetTextMods( unsigned int uiIndex ) const;

	const char *GetClipboardString( void ) const;
	void SetClipboardString( const char *sString );

private:
	void SetCursorPosition( glm::vec2 vec2CursorPosition );
	void SetCursorDelta( glm::vec2 vec2CursorPosition );

private:
	ECursorShape m_eCursorShape;
	GLFWcursor *m_pCursors[(EBaseEnum)ECursorShape::i_count];

	CKeyBind m_KeyBinds[(EBaseEnum)EKeyCodes::i_count];
	bool m_bPrevKeyDown[(EBaseEnum)EKeyCodes::i_count];
	bool m_bKeyDown[(EBaseEnum)EKeyCodes::i_count];

	glm::vec2 m_vec2CursorPosition;
	glm::vec2 m_vec2NormalizedCursorPosition;

	glm::vec2 m_vec2CursorDelta;
	glm::vec2 m_vec2NormalizedCursorDelta;

	std::vector<SInputChar> m_sText;

	bool m_bLockCursor;
	bool m_bLockKeyboard;
};

#endif // INPUTMANAGER_H