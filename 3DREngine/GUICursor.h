#ifndef GUICURSOR_H
#define GUICURSOR_H

#include "Global.h"
#include "GUIPanel.h"
#include "GUIText.h"

class CGUICursor : public CGUIPanel
{
public:
	DECLARE_CLASS( CGUICursor, CGUIPanel )

	DECLARE_LINKED_CLASS()

	CGUICursor();

	virtual bool ShouldDraw( void ) const;

	virtual void PreRender( void );

	void SetGUIText( CGUIText *pGUIText );

	bool IsHighlighting( void ) const;
	bool IsIndexWithinBounds( void ) const;
	bool IsCursorWithinBounds( void ) const;

	void MoveToStart( bool bHighlight = false );
	void MoveHorizontal( int iDelta, bool bHighlight = false );
	void MoveVertical( int iDelta, bool bHighlight = false );

	void Insert( char cChar );
	void Insert( const char *sString );
	void Delete( void );
	void Backspace( void );

	void Click( const glm::vec2 &vec2Cursor );
	void Drag( const glm::vec2 &vec2Cursor );
	void Release( const glm::vec2 &vec2Cursor );

	void StartBlinking( void );
	void StopBlinking( void );

private:
	void SetFromCursor( const glm::vec2 &vec2Cursor );
	void SyncCursorToIndex( void );
	void SyncIndexToCursor( void );

	void OnStateChange( void );
	void PreventHighlight( void );
	void CalculateShouldDisplay( void );

	virtual const glm::vec2 &GetAlignFactor( void ) const;
	virtual const glm::vec2 &GetOffset( void ) const;

private:
	CGUIText *m_pGUIText;

	unsigned int m_uiStartCursorIndex;
	unsigned int m_uiCursorIndex;

	glm::ivec2 m_vec2StartCursorPosition;
	glm::ivec2 m_vec2CursorPosition;

	glm::vec2 m_vec2Offset;

	bool m_bShouldDisplay;

	float m_flNextBlink;
	bool m_bToggleDisplay;
};

#endif // GUICURSOR_H