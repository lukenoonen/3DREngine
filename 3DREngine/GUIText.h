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

struct SVertex2D
{
	glm::vec2 vec2Position;
	glm::vec2 vec2TexCoords;
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

	void Write( CGUITextData &tFormatInfo );

	float GetJustification( float flPosition );
	void CalculateMinMax( char cChar, float flPosition, float flJustification, glm::vec2 &vec2Min, glm::vec2 &vec2Max, glm::vec2 &vec2TexMin, glm::vec2 &vec2TexMax );

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
	std::stack<SGUITextChar> m_tcText;
};

class CGUITextData
{
public:
	DECLARE_CLASS_NOBASE( CGUITextData )

	CGUITextData();

	void Reset( void );
	void Return( void );
	void Record( float flCharOffset );

	void SetLineSize( float flLineSize );
	void Adjust( unsigned int uiLine, float flJustification );

	void Push( const glm::vec2 &vec2Min, const glm::vec2 &vec2Max, const glm::vec2 &vec2TexMin, const glm::vec2 &vec2TexMax );

	void CreateText( void );
	void DeleteText( void );
	void Draw( void );

	unsigned int GetDisplayTextLength( void ) const;
	unsigned int GetLineCount( void ) const;
	unsigned int GetCharCount( unsigned int uiLine ) const;

	float GetLineOffset( unsigned int uiLine ) const;
	float GetCharOffset( unsigned int uiLine, unsigned int uiChar ) const;
	float GetPointSize( void ) const;

private:
	float m_flLineSize;
	unsigned int m_uiCharOffsetsCount;
	std::vector<std::vector<float>> m_flCharOffsets;

	std::vector<SVertex2D> m_verVertices;

	GLuint m_glVAO;
	GLuint m_glVBO;
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

	virtual void Draw( void );
	virtual bool ShouldDraw( void ) const;

	bool Insert( char cChar, unsigned int uiIndex );
	bool Insert( const char *sString, unsigned int uiIndex );
	bool Delete( unsigned int uiCount, unsigned int uiIndex );
	bool Backspace( unsigned int uiCount, unsigned int uiIndex );

	unsigned int GetTextLength( void ) const;

	unsigned int GetDisplayTextLength( void ) const;
	unsigned int GetLineCount( void ) const;
	unsigned int GetCharCount( unsigned int uiLine ) const;
	float GetLineOffset( unsigned int uiLine ) const;
	float GetCharOffset( unsigned int uiLine, unsigned int uiChar ) const;

	float GetPointSize( void ) const;

	void SetText( const char *sText );
	const char *GetText( void );

private:
	void RecalculateText( void );
	void CalculateText( void );
	void DeleteText( void );

	virtual const glm::vec2 &GetAlignFactor( void ) const;

private:
	char *m_sText;
	std::vector<char> m_cText;

	CGUITextData m_tTextInfo;

	CHandle<CTextMaterial> m_hTextMaterial;

	glm::vec2 m_vec2Bounds;
	float m_flLineScale;
	ETextAlign m_eTextAlign;

	glm::vec2 m_vec2Offset;
};

#endif // GUITEXT_H