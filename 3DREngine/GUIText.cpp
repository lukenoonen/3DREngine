#include "GUIText.h"
#include "RenderManager.h"

CGUITextChar::CGUITextChar( const glm::vec2 &vec2Offset, char cChar )
{
	m_vec2Offset = vec2Offset;
	m_cChar = cChar;
}


void CGUITextChar::Justify( float flJustification )
{
	m_vec2Offset.x += flJustification;
}

void CGUITextChar::Draw( CFont *pFont ) const
{
	pRenderManager->SetUniform( "u_vecOffset", m_vec2Offset );
	pFont->DrawChar( m_cChar );
}

CGUITextGeneration::CGUITextGeneration( const std::vector<char> &cText, const glm::vec2 &vec2Bounds, ETextAlign eTextAlign, float flTextScale, float flLineSize, CFont *pFont ) : m_cText( cText ), m_vec2Bounds( vec2Bounds )
{
	m_eTextAlign = eTextAlign;
	m_flTextScale = flTextScale;
	m_flLineSize = flLineSize;
	m_pFont = pFont;

	m_vec2Cursor = g_vec2Zero;
	m_uiCursor = 0;
	m_flLineEnd = 0.0f;
	m_uiLines = 0;
}

void CGUITextGeneration::Generate( CGUITextData &tFormatInfo )
{
	tFormatInfo.SetLineSize( m_flLineSize );
	tFormatInfo.SetTextScale( m_flTextScale );
	Read( tFormatInfo );
}

void CGUITextGeneration::Read( CGUITextData &tFormatInfo )
{
	tFormatInfo.Reset();

	while (m_uiCursor < (unsigned int)m_cText.size() - 1)
	{
		char cChar = m_cText[m_uiCursor];

		unsigned int uiTarget;
		float flAdvance;
		bool bReturn;

		bReturn = Scan( uiTarget, flAdvance );

		if (m_eTextAlign != ETextAlign::t_continuous && bReturn)
		{
			if (Return( tFormatInfo ))
				break;
		}

		if (!UTIL_isspace( cChar ))
			m_flLineEnd = m_vec2Cursor.x + flAdvance;

		while (m_uiCursor < uiTarget)
		{
			bool bCharReturn = Process( tFormatInfo );
			if (bCharReturn)
			{
				if (Return( tFormatInfo ))
					break;
				m_flLineEnd = 0.0f;
			}
			m_uiCursor++;
		}

		if (m_eTextAlign == ETextAlign::t_continuous && bReturn)
		{
			if (Return( tFormatInfo ))
				break;
		}

		if (m_uiCursor != uiTarget)
			break;
	}

	tFormatInfo.Adjust( m_uiLines, GetJustification( m_flLineEnd ) );
}

bool CGUITextGeneration::Scan( unsigned int &uiTarget, float &flAdvance )
{
	char cChar = m_cText[m_uiCursor];
	bool bIsSpace = UTIL_isspace( cChar );

	uiTarget = m_uiCursor;

	flAdvance = GetAdvance( cChar );
	uiTarget++;

	if (!bIsSpace)
	{
		switch (m_eTextAlign)
		{
		case ETextAlign::t_left:
		case ETextAlign::t_center:
		case ETextAlign::t_right:
		case ETextAlign::t_block:
		{
			while (!UTIL_isspace( m_cText[uiTarget] ) && flAdvance < m_vec2Bounds.x)
			{
				flAdvance += GetAdvance( m_cText[uiTarget] );
				uiTarget++;
			}
			break;
		}
		}
	}

	return !bIsSpace && m_vec2Cursor.x + flAdvance >= m_vec2Bounds.x && m_vec2Cursor.x != 0.0f;
}

bool CGUITextGeneration::Return( CGUITextData &tFormatInfo )
{
	m_vec2Cursor.y += m_flLineSize;

	tFormatInfo.Adjust( m_uiLines, GetJustification( m_flLineEnd ) );
	tFormatInfo.Return();

	if (m_vec2Cursor.y >= m_vec2Bounds.y) // TODO: figure out a better solution to hide the cursor while out of bounds
		return true;

	m_uiLines++;

	m_vec2Cursor.x = 0.0f;
	return false;
}

bool CGUITextGeneration::Process( CGUITextData &tFormatInfo )
{
	char cChar = m_cText[m_uiCursor];
	float flAdvance = GetAdvance( cChar );
	tFormatInfo.Record( cChar, m_vec2Cursor, flAdvance );
	m_vec2Cursor.x += flAdvance;
	return cChar == '\n';
}

float CGUITextGeneration::GetAdvance( char cChar )
{
	switch (cChar)
	{
	case ' ':
	{
		return m_pFont->GetAdvance( ' ', m_flTextScale );
	}
	case '\t':
	{
		float flTabAdvance = m_pFont->GetAdvance( ' ', m_flTextScale ) * 4.0f;
		float flCursorDelta = (ceilf( m_vec2Cursor.x / flTabAdvance )) * flTabAdvance - m_vec2Cursor.x;
		if (flCursorDelta == 0.0f)
			flCursorDelta = flTabAdvance;
		return flCursorDelta;
	}
	case '\n':
	case '\0':
	{
		return m_pFont->GetAdvance( ' ', m_flTextScale );
	}
	}

	return m_pFont->GetAdvance( cChar, m_flTextScale );
}

float CGUITextGeneration::GetJustification( float flPosition )
{
	switch (m_eTextAlign)
	{
	case ETextAlign::t_right:
	{
		return m_vec2Bounds.x - flPosition;
	}
	case ETextAlign::t_center:
	{
		return (m_vec2Bounds.x - flPosition) * 0.5f;
	}
	}
	return 0.0f;
}

CGUITextData::CGUITextData()
{
	m_uiCharOffsetsCount = 0;
	m_uiTextCharsCount = 0;
}

void CGUITextData::Reset( void )
{
	m_uiCharOffsetsCount = 0;
	m_flCharOffsets.clear();

	m_uiTextCharsCount = 0;
	m_tcTextChars.clear();

	Return();
}

void CGUITextData::Return( void )
{
	m_flCharOffsets.emplace_back();
	m_flCharOffsets.back().push_back( 0.0f );

	m_tcTextChars.emplace_back();
}

void CGUITextData::Record( char cChar, const glm::vec2 &vec2Offset, float flAdvance )
{
	if (!UTIL_isspace( cChar ))
	{
		m_uiTextCharsCount++;
		m_tcTextChars.back().emplace_back( vec2Offset + glm::vec2( 0.0f, m_flLineSize ), cChar );
	}

	m_uiCharOffsetsCount++;
	m_flCharOffsets.back().push_back( vec2Offset.x + flAdvance );
}

void CGUITextData::SetTextScale( float flTextScale )
{
	m_flTextScale = flTextScale;
}

void CGUITextData::SetLineSize( float flLineSize )
{
	m_flLineSize = flLineSize;
}

void CGUITextData::Adjust( unsigned int uiLine, float flJustification )
{
	std::vector<float> &flCharOffsets = m_flCharOffsets[uiLine];
	for (unsigned int i = 0; i < flCharOffsets.size(); i++)
		flCharOffsets[i] += flJustification;

	std::vector<CGUITextChar> &tcTextChars = m_tcTextChars[uiLine];
	for (unsigned int i = 0; i < tcTextChars.size(); i++)
		tcTextChars[i].Justify( flJustification );
}

void CGUITextData::Draw( CFont *pFont )
{
	pRenderManager->SetUniform( "u_flTextScale", m_flTextScale );

	for (unsigned int i = 0; i < m_tcTextChars.size(); i++)
	{
		std::vector<CGUITextChar> &tcTextChars = m_tcTextChars[i];
		for (unsigned int j = 0; j < tcTextChars.size(); j++)
			tcTextChars[j].Draw( pFont );
	}
}

unsigned int CGUITextData::GetDisplayTextLength( void ) const
{
	return m_uiCharOffsetsCount;
}

unsigned int CGUITextData::GetLineCount( void ) const
{
	return (unsigned int)m_flCharOffsets.size();
}

unsigned int CGUITextData::GetCharCount( unsigned int uiLine ) const
{
	return (unsigned int)m_flCharOffsets[uiLine].size() - 1;
}

float CGUITextData::GetLineOffset( unsigned int uiLine ) const
{
	return (float)(uiLine + 1) * m_flLineSize;
}

float CGUITextData::GetCharOffset( unsigned int uiLine, unsigned int uiChar ) const
{
	return m_flCharOffsets[uiLine][uiChar];
}

float CGUITextData::GetPointSize( void ) const
{
	return m_flLineSize;
}

DEFINE_DATADESC( CGUIText )

	DEFINE_FIELD( DataField, char *, m_sText, "text", FL_NONE ) // TODO: make std::vector<char> work like these strings

	DEFINE_FIELD( LinkedDataField, CHandle<CTextMaterial>, m_hTextMaterial, "textmaterial", FL_REQUIRED )

	DEFINE_FIELD( DataField, glm::vec2, m_vec2Bounds, "bounds", FL_NONE )
	DEFINE_FIELD( DataField, float, m_flLineScale, "linescale", FL_NONE )
	DEFINE_FIELD( DataField, ETextAlign, m_eTextAlign, "textalign", FL_NONE )

END_DATADESC()

DEFINE_LINKED_CLASS( CGUIText, text_gui )

CGUIText::CGUIText()
{
	m_sText = NULL;
	m_bTextModified = false;

	m_vec2Bounds = g_vec2One;
	m_flLineScale = 1.0f;
	m_eTextAlign = ETextAlign::t_left;
}

CGUIText::~CGUIText()
{
	if (m_sText)
		delete[] m_sText;
}

bool CGUIText::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (m_sText)
		SetText( m_sText );
	else
		SetText( "" );

	return true;
}

void CGUIText::PreThink( void )
{
	if (cv_r_windowsize.WasDispatched())
		ModifyText();

	BaseClass::PreThink();
}

void CGUIText::PreRender( void )
{
	if (IsTextModified())
		CalculateText();

	BaseClass::PreRender();
}

void CGUIText::Draw( void )
{
	BaseClass::Draw();

	m_hTextMaterial->Apply();
	m_tTextInfo.Draw( m_hTextMaterial->GetFont() );
}

bool CGUIText::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_hTextMaterial->ShouldApply();
}

void CGUIText::Append( char cChar )
{
	m_cText.insert( m_cText.begin() + GetTextLength(), cChar );
	ModifyText();
}

bool CGUIText::Insert( char cChar, unsigned int uiIndex )
{
	if (uiIndex <= GetTextLength())
	{
		m_cText.insert( m_cText.begin() + uiIndex, cChar );
		ModifyText();
		return true;
	}

	return false;
}

void CGUIText::Append( const char *sString )
{
	std::vector<char> cTextToAdd;
	while (*sString)
	{
		char cCharToAdd = *sString;
		switch (*sString)
		{
		case '\r':
			break;
		default:
			cTextToAdd.push_back( cCharToAdd );
			break;
		}
		sString++;
	}
	m_cText.insert( m_cText.begin() + GetTextLength(), cTextToAdd.begin(), cTextToAdd.end() );
	ModifyText();
}

bool CGUIText::Insert( const char *sString, unsigned int uiIndex )
{
	if (uiIndex <= GetTextLength())
	{
		std::vector<char> cTextToAdd;
		while (*sString)
		{
			char cCharToAdd = *sString;
			switch (*sString)
			{
			case '\r':
				break;
			default:
				cTextToAdd.push_back( cCharToAdd );
				break;
			}
			sString++;
		}
		m_cText.insert( m_cText.begin() + uiIndex, cTextToAdd.begin(), cTextToAdd.end() );
		ModifyText();
		return true;
	}

	return false;
}

bool CGUIText::Delete( unsigned int uiCount, unsigned int uiIndex )
{
	if (uiIndex + uiCount - 1 < GetTextLength())
	{
		m_cText.erase( m_cText.begin() + uiIndex, m_cText.begin() + uiIndex + uiCount );
		ModifyText();
		return true;
	}

	return false;
}

bool CGUIText::Backspace( unsigned int uiCount, unsigned int uiIndex )
{
	if (uiIndex >= uiCount)
	{
		Delete( uiCount, uiIndex - uiCount );
		return true;
	}

	return false;
}

const char *CGUIText::GetText( void )
{
	if (m_sText)
		delete[] m_sText;

	unsigned int uiFullTextSize = (unsigned int)m_cText.size();
	m_sText = new char[uiFullTextSize];
	for (unsigned int i = 0; i < uiFullTextSize; i++)
		m_sText[i] = m_cText[i];

	return m_sText;
}

void CGUIText::SetText( const char *sText )
{
	m_cText.clear();
	const char *pSearch = sText;
	while (*pSearch) m_cText.push_back( *pSearch++ );
	m_cText.push_back( '\0' );
	ModifyText();
}

unsigned int CGUIText::GetTextLength( void ) const
{
	return (unsigned int)m_cText.size() - 1;
}

unsigned int CGUIText::GetDisplayTextLength( void ) const
{
	return m_tTextInfo.GetDisplayTextLength();
}

unsigned int CGUIText::GetLineCount( void ) const
{
	return m_tTextInfo.GetLineCount();
}

unsigned int CGUIText::GetCharCount( unsigned int uiLine ) const
{
	return m_tTextInfo.GetCharCount( uiLine );
}

float CGUIText::GetLineOffset( unsigned int uiLine ) const
{
	return m_tTextInfo.GetLineOffset( uiLine );
}

float CGUIText::GetCharOffset( unsigned int uiLine, unsigned int uiChar ) const
{
	return m_tTextInfo.GetCharOffset( uiLine, uiChar );
}

float CGUIText::GetPointSize( void ) const
{
	return m_tTextInfo.GetPointSize();
}

const glm::vec2 &CGUIText::GetBounds( void ) const
{
	return m_vec2Bounds;
}

void CGUIText::ModifyText( void )
{
	if (!m_bTextModified)
		m_bTextModified = true;
}

bool CGUIText::IsTextModified( void ) const
{
	return m_bTextModified;
}

// TODO: clean up and finish (BLOCK TEXT NOT DONE)
void CGUIText::CalculateText( void )
{
	CFont *pFont = m_hTextMaterial->GetFont();
	float flTextScale = 2.0f / (float)(HasFlags( fl_absolute.GetFlag() ) ? DEFAULT_SCR_WIDTH : cv_r_windowsize.GetValue().x);
	float flLineSize = (float)pFont->GetFontSize() * m_flLineScale * flTextScale;

	CGUITextGeneration tgTextGeneration( m_cText, m_vec2Bounds, m_eTextAlign, flTextScale, flLineSize, pFont );
	tgTextGeneration.Generate( m_tTextInfo );

	m_bTextModified = false;
}

const glm::vec2 &CGUIText::GetAlignFactor( void ) const
{
	return m_vec2Bounds;
}