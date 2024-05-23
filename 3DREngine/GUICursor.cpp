#include "GUICursor.h"
#include "TimeManager.h"

/*DEFINE_DATADESC(CGUICursor)

END_DATADESC()*/

DEFINE_LINKED_CLASS( CGUICursor, cursor_gui )

CGUICursor::CGUICursor()
{
	m_pGUIText = NULL;

	m_uiStartCursorIndex = 0;
	m_uiCursorIndex = 0;

	m_vec2CursorPosition = g_ivec2Zero;
	m_vec2StartCursorPosition = g_ivec2Zero;

	m_vec2Offset = g_vec2Zero;

	m_bShouldDisplay = false;
	m_bToggleDisplay = true;
}

bool CGUICursor::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_bShouldDisplay && m_bToggleDisplay;
}

void CGUICursor::PreRender( void )
{
	if (m_flNextBlink <= pTimeManager->GetTime())
	{
		m_flNextBlink = pTimeManager->GetTime() + 0.5f;
		m_bToggleDisplay = !m_bToggleDisplay;
	}

	if (m_bShouldDisplay && m_bToggleDisplay)
	{
		glm::vec2 vec2Scale = glm::vec2( 2.0f / (float)cv_r_windowsize.GetValue().x, m_pGUIText->GetPointSize() );
		SetScale( glm::vec3( vec2Scale.x, 1.0f, vec2Scale.y ) );
		m_vec2Offset = glm::vec2( m_pGUIText->GetCharOffset( (unsigned int)m_vec2CursorPosition.y, (unsigned int)m_vec2CursorPosition.x ), m_pGUIText->GetLineOffset( (unsigned int)m_vec2CursorPosition.y ) - m_pGUIText->GetPointSize() );
	}
		
	BaseClass::PreRender();
}

void CGUICursor::SetGUIText( CGUIText *pGUIText )
{
	SetPosition( g_vec3Zero );
	SetRotation( g_vec3Zero );
	SetScale( g_vec3One );
	SetParentRelative( pGUIText );

	m_pGUIText = pGUIText;
}

bool CGUICursor::IsHighlighting( void ) const
{
	return m_uiCursorIndex != m_uiStartCursorIndex;
}

bool CGUICursor::IsIndexWithinBounds( void ) const
{
	return m_uiCursorIndex <= m_pGUIText->GetDisplayTextLength();
}

bool CGUICursor::IsCursorWithinBounds( void ) const
{
	return m_vec2CursorPosition.y < (int)m_pGUIText->GetLineCount() && m_vec2CursorPosition.x <= (int)m_pGUIText->GetCharCount( m_vec2CursorPosition.y );
}

void CGUICursor::MoveHorizontal( int iDelta, bool bHighlight /* = false */ )
{
	unsigned int uiNewCursorIndex = m_uiCursorIndex + iDelta;
	if (uiNewCursorIndex <= m_pGUIText->GetTextLength())
	{
		m_uiCursorIndex = uiNewCursorIndex;
		if (IsIndexWithinBounds())
			SyncCursorToIndex();
		CalculateShouldDisplay();
		if (!bHighlight)
			PreventHighlight();
	}
}

void CGUICursor::MoveVertical( int iDelta, bool bHighlight /* = false */ )
{
	CalculateShouldDisplay();
	if (!bHighlight)
		PreventHighlight();
}

void CGUICursor::Insert( char cChar )
{
	if (IsHighlighting())
		Delete();

	if (m_pGUIText->Insert( cChar, m_uiCursorIndex ))
		MoveHorizontal( 1 );

	OnStateChange();
}

void CGUICursor::Insert( const char *sString )
{
	if (IsHighlighting())
		Delete();

	if (m_pGUIText->Insert( sString, m_uiCursorIndex )) // TODO: FIX THIS
		MoveHorizontal( (int)UTIL_strlen( sString ) );

	OnStateChange();
}

void CGUICursor::Delete( void )
{
	if (IsHighlighting())
	{
		if (m_uiCursorIndex > m_uiStartCursorIndex)
		{
			UTIL_swap( m_uiCursorIndex, m_uiStartCursorIndex );
			UTIL_swap( m_vec2CursorPosition, m_vec2StartCursorPosition );
		}

		m_pGUIText->Delete( m_uiStartCursorIndex - m_uiCursorIndex, m_uiCursorIndex );

		PreventHighlight();
	}
	else
	{
		m_pGUIText->Delete( 1, m_uiCursorIndex );
	}

	OnStateChange();
}

void CGUICursor::Backspace( void )
{
	if (IsHighlighting())
		Delete();
	else if (m_pGUIText->Backspace( 1, m_uiCursorIndex ))
		MoveHorizontal( -1 );

	OnStateChange();
}

void CGUICursor::Click( const glm::vec2 &vec2Cursor )
{
	SetFromCursor( vec2Cursor );

	PreventHighlight();
}

void CGUICursor::Drag( const glm::vec2 &vec2Cursor )
{
	SetFromCursor( vec2Cursor );
}

void CGUICursor::Release( const glm::vec2 &vec2Cursor )
{
	SetFromCursor( vec2Cursor );
}

void CGUICursor::StartBlinking( void )
{
	m_bToggleDisplay = true;
	m_flNextBlink = pTimeManager->GetTime() + 0.5f;
}

void CGUICursor::StopBlinking( void )
{
	m_bToggleDisplay = false;
	m_flNextBlink = INFINITY;
}

void CGUICursor::SetFromCursor( const glm::vec2 &vec2Cursor )
{
	glm::ivec2 vec2PrevCursorPosition = m_vec2CursorPosition;

	m_uiCursorIndex = m_pGUIText->GetDisplayTextLength() + 1;
	m_vec2CursorPosition.y = m_pGUIText->GetLineCount() - 1;
	while (m_vec2CursorPosition.y > 0)
	{
		if (vec2Cursor.y >= m_pGUIText->GetLineOffset( m_vec2CursorPosition.y - 1 ))
			break;
		m_uiCursorIndex -= m_pGUIText->GetCharCount( m_vec2CursorPosition.y );
		m_vec2CursorPosition.y--;
	}

	m_vec2CursorPosition.x = m_pGUIText->GetCharCount( m_vec2CursorPosition.y );
	while (m_vec2CursorPosition.x > 0)
	{
		if (vec2Cursor.x >= m_pGUIText->GetCharOffset( m_vec2CursorPosition.y, m_vec2CursorPosition.x ))
			break;
		m_uiCursorIndex--;
		m_vec2CursorPosition.x--;
	}
	m_uiCursorIndex--;
	CalculateShouldDisplay();

	if (m_vec2CursorPosition != vec2PrevCursorPosition)
		StartBlinking();
}

void CGUICursor::SyncCursorToIndex( void )
{
	int iCursorIndex = (int)m_uiCursorIndex;
	int iLineCharCount = 0;
	int iLineCount = (int)m_pGUIText->GetLineCount();
	m_vec2CursorPosition = g_ivec2Zero;
	while (m_vec2CursorPosition.y < iLineCount)
	{
		iLineCharCount = (int)m_pGUIText->GetCharCount( m_vec2CursorPosition.y );
		if (iCursorIndex <= iLineCharCount)
			break;
		iCursorIndex -= iLineCharCount;
		m_vec2CursorPosition.y++;
	}
	if (m_vec2CursorPosition.y < iLineCount - 1 && iCursorIndex == iLineCharCount)
	{
		m_vec2CursorPosition.y++;
		iCursorIndex = 0;
	}
	m_vec2CursorPosition.x = iCursorIndex;
}

void CGUICursor::SyncIndexToCursor( void )
{

}

void CGUICursor::OnStateChange( void )
{
	StartBlinking();
}

void CGUICursor::PreventHighlight( void )
{
	m_uiStartCursorIndex = m_uiCursorIndex;
	m_vec2StartCursorPosition = m_vec2CursorPosition;
}

void CGUICursor::CalculateShouldDisplay( void )
{
	m_bShouldDisplay = IsIndexWithinBounds() && IsCursorWithinBounds();
}

const glm::vec2 &CGUICursor::GetOffset( void ) const
{
	return m_vec2Offset;
}