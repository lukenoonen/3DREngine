#ifndef GUITEXT_H
#define GUITEXT_H

#include "Global.h"
#include "BaseBillboard.h"
#include "TextMaterial.h"

class CGUITextData;
class CGUIText;

enum class ETextAlign : EBaseEnum
{
	t_left = 0,
	t_center,
	t_right,
	t_block,
	t_continuous,

	i_count,
	i_invalid = i_count,
};

static const char *g_sETextAlignNames[] =
{
	"left",
	"center",
	"right",
	"block",
	"continuous",
};

DEFINE_ENUM_NAMES( ETextAlign, g_sETextAlignNames )

DEFINE_ENUM_GETVALUE( ETextAlign )

struct SGUITextChar
{
	char cChar;
	float flXPos;
};

class CGUITextChar
{
public:
	DECLARE_CLASS_NOBASE( CGUITextChar )

	CGUITextChar( const glm::vec2 &vec2Offset, char cChar );

	void Justify( float flJustification );

	void Draw( CFont *pFont ) const;

private:
	glm::vec2 m_vec2Offset;
	char m_cChar;
};

class CGUITextGeneration
{
public:
	DECLARE_CLASS_NOBASE( CGUITextGeneration )

	CGUITextGeneration( const std::vector<char> &cText, const glm::vec2 &vec2Bounds, ETextAlign eTextAlign, float flTextScale, float flLineSize, CFont *pFont );

	void Generate( CGUITextData &tFormatInfo );

private:
	void Read( CGUITextData &tFormatInfo );

	bool Scan( unsigned int &uiTarget, float &flAdvance );
	bool Return( CGUITextData &tFormatInfo );
	bool Process( CGUITextData &tFormatInfo );

	float GetAdvance( char cChar );
	float GetJustification( float flPosition );
	
private:
	const std::vector<char> &m_cText;
	const glm::vec2 &m_vec2Bounds;

	ETextAlign m_eTextAlign;
	float m_flTextScale;
	float m_flLineSize;
	CFont *m_pFont;

	glm::vec2 m_vec2Cursor;
	unsigned int m_uiCursor;
	float m_flLineEnd;
	unsigned int m_uiLines;
};

class CGUITextData
{
public:
	DECLARE_CLASS_NOBASE( CGUITextData )

	CGUITextData();

	void Reset( void );
	void Return( void );
	void Record( char cChar, const glm::vec2 &vec2Offset, float flAdvance );

	void SetTextScale( float flTextScale );
	void SetLineSize( float flLineSize );
	void Adjust( unsigned int uiLine, float flJustification );

	void Draw( CFont *pFont );

	unsigned int GetDisplayTextLength( void ) const;
	unsigned int GetLineCount( void ) const;
	unsigned int GetCharCount( unsigned int uiLine ) const;

	float GetLineOffset( unsigned int uiLine ) const;
	float GetCharOffset( unsigned int uiLine, unsigned int uiChar ) const;
	float GetPointSize( void ) const;

private:
	float m_flTextScale;
	float m_flLineSize;

	unsigned int m_uiCharOffsetsCount;
	std::vector<std::vector<float>> m_flCharOffsets;

	unsigned int m_uiTextCharsCount;
	std::vector<std::vector<CGUITextChar>> m_tcTextChars;
};

class CGUIText : public CBaseBillboard
{
public:
	DECLARE_CLASS( CGUIText, CBaseBillboard )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CGUIText();
	virtual ~CGUIText();

	virtual bool Init( void );

	virtual void PreThink( void );

	virtual void PreRender( void );

	virtual void Draw( void );
	virtual bool ShouldDraw( void ) const;

	void Append( char cChar );
	bool Insert( char cChar, unsigned int uiIndex );
	void Append( const char *sString );
	bool Insert( const char *sString, unsigned int uiIndex );
	bool Delete( unsigned int uiCount, unsigned int uiIndex );
	bool Backspace( unsigned int uiCount, unsigned int uiIndex );
	void SetText( const char *sText );
	const char *GetText( void );

	unsigned int GetTextLength( void ) const;

	unsigned int GetDisplayTextLength( void ) const;
	unsigned int GetLineCount( void ) const;
	unsigned int GetCharCount( unsigned int uiLine ) const;

	float GetLineOffset( unsigned int uiLine ) const;
	float GetCharOffset( unsigned int uiLine, unsigned int uiChar ) const;
	float GetPointSize( void ) const;

	const glm::vec2 &GetBounds( void ) const;

private:
	void ModifyText( void );
	bool IsTextModified( void ) const;
	void CalculateText( void );

	virtual const glm::vec2 &GetAlignFactor( void ) const;

private:
	char *m_sText;
	std::vector<char> m_cText;
	bool m_bTextModified;

	CGUITextData m_tTextInfo;

	CHandle<CTextMaterial> m_hTextMaterial;

	glm::vec2 m_vec2Bounds;
	float m_flLineScale;
	ETextAlign m_eTextAlign;

	glm::vec2 m_vec2Offset;
};

#endif // GUITEXT_H