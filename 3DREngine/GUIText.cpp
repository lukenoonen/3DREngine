#include "GUIText.h"
#include "RenderManager.h"

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
	Read( tFormatInfo );
	Write( tFormatInfo );
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

	m_uiLines++;
	m_tcText.push( { '\0', m_flLineEnd } );
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

	tFormatInfo.Return();

	if (m_vec2Cursor.y >= m_vec2Bounds.y) // TODO: figure out a better solution to hide the cursor while out of bounds
		return true;

	m_uiLines++;
	m_tcText.push( { '\0', m_flLineEnd } );

	m_vec2Cursor.x = 0.0f;
	return false;
}

bool CGUITextGeneration::Process( CGUITextData &tFormatInfo )
{
	char cChar = m_cText[m_uiCursor];

	if (m_vec2Cursor.x <= m_vec2Bounds.x && !UTIL_isspace( cChar ))
		m_tcText.push( { cChar, m_vec2Cursor.x } );

	m_vec2Cursor.x += GetAdvance( cChar );
	tFormatInfo.Record( m_vec2Cursor.x );

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

void CGUITextGeneration::Write( CGUITextData &tFormatInfo )
{
	float flJustification = 0.0f;
	while (!m_tcText.empty())
	{
		char cChar = m_tcText.top().cChar;
		float flPosition = m_tcText.top().flXPos;
		if (cChar == '\0')
		{
			m_uiLines--;
			flJustification = GetJustification( flPosition );
			tFormatInfo.Adjust( m_uiLines, flJustification );
		}
		else
		{
			glm::vec2 vec2Min, vec2Max;
			glm::vec2 vec2TexMin, vec2TexMax;
			CalculateMinMax( cChar, flPosition, flJustification, vec2Min, vec2Max, vec2TexMin, vec2TexMax );
			tFormatInfo.Push( vec2Min, vec2Max, vec2TexMin, vec2TexMax );
		}

		m_tcText.pop();
	}
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

void CGUITextGeneration::CalculateMinMax( char cChar, float flPosition, float flJustification, glm::vec2 &vec2Min, glm::vec2 &vec2Max, glm::vec2 &vec2TexMin, glm::vec2 &vec2TexMax )
{
	glm::vec2 vec2TextOrigin = glm::vec2( flPosition + flJustification, (float)(m_uiLines + 1) * m_flLineSize );

	vec2Min = vec2TextOrigin + m_pFont->GetOffset( cChar, m_flTextScale );
	vec2Max = vec2Min + m_pFont->GetSize( cChar, m_flTextScale );

	glm::vec2 vec2BoundsAdjust = g_vec2One;
	glm::vec2 vec2BoundsShift = g_vec2Zero;

	if (vec2Min.x < 0.0f)
	{
		float flScale = 1.0f / (vec2Max.x - vec2Min.x);
		vec2BoundsAdjust.x = vec2Max.x * flScale;
		vec2BoundsShift.x = -vec2Min.x * flScale;
		vec2Min.x = 0.0f;
	}
	if (vec2Max.x > m_vec2Bounds.x)
	{
		vec2BoundsAdjust.x = (m_vec2Bounds.x - vec2Min.x) / (vec2Max.x - vec2Min.x);
		vec2Max.x = m_vec2Bounds.x;
	}

	if (vec2Min.y < 0.0f)
	{
		float flScale = 1.0f / (vec2Max.y - vec2Min.y);
		vec2BoundsAdjust.y = vec2Max.y / flScale;
		vec2BoundsShift.y = -vec2Min.y * flScale;
		vec2Min.y = 0.0f;
	}
	if (vec2Max.y > m_vec2Bounds.y)
	{
		vec2BoundsAdjust.y = (m_vec2Bounds.y - vec2Min.y) / (vec2Max.y - vec2Min.y);
		vec2Max.y = m_vec2Bounds.y;
	}

	vec2Min *= g_vec2FlipVertical;
	vec2Max *= g_vec2FlipVertical;

	vec2TexMin = m_pFont->GetNormalizedPosition( cChar ) + m_pFont->GetNormalizedSize( cChar ) * vec2BoundsShift;
	vec2TexMax = vec2TexMin + m_pFont->GetNormalizedSize( cChar ) * vec2BoundsAdjust;
}

CGUITextData::CGUITextData()
{
	m_glVAO = 0;
	m_glVBO = 0;
}

void CGUITextData::Reset( void )
{
	m_uiCharOffsetsCount = 0;
	m_flCharOffsets.clear();

	m_verVertices.clear();

	Return();
}

void CGUITextData::Return( void )
{
	m_flCharOffsets.emplace_back();
	m_flCharOffsets.back().push_back( 0.0f );
}

void CGUITextData::Record( float flCharOffset )
{
	m_uiCharOffsetsCount++;
	m_flCharOffsets.back().push_back( flCharOffset );
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
}

void CGUITextData::Push( const glm::vec2 &vec2Min, const glm::vec2 &vec2Max, const glm::vec2 &vec2TexMin, const glm::vec2 &vec2TexMax )
{
	m_verVertices.push_back( { glm::vec2( vec2Min.x, vec2Min.y ), glm::vec2( vec2TexMin.x, vec2TexMin.y ) } );
	m_verVertices.push_back( { glm::vec2( vec2Min.x, vec2Max.y ), glm::vec2( vec2TexMin.x, vec2TexMax.y ) } );
	m_verVertices.push_back( { glm::vec2( vec2Max.x, vec2Max.y ), glm::vec2( vec2TexMax.x, vec2TexMax.y ) } );
	m_verVertices.push_back( { glm::vec2( vec2Max.x, vec2Max.y ), glm::vec2( vec2TexMax.x, vec2TexMax.y ) } );
	m_verVertices.push_back( { glm::vec2( vec2Max.x, vec2Min.y ), glm::vec2( vec2TexMax.x, vec2TexMin.y ) } );
	m_verVertices.push_back( { glm::vec2( vec2Min.x, vec2Min.y ), glm::vec2( vec2TexMin.x, vec2TexMin.y ) } );
}

void CGUITextData::CreateText( void )
{
	glGenVertexArrays( 1, &m_glVAO );
	glGenBuffers( 1, &m_glVBO );

	glBindVertexArray( m_glVAO );
	glBindBuffer( GL_ARRAY_BUFFER, m_glVBO );
	glBufferData( GL_ARRAY_BUFFER, (GLsizeiptr)(m_verVertices.size() * sizeof( SVertex2D )), &m_verVertices[0], GL_STATIC_DRAW );

	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( SVertex2D ), (void *)0 );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( SVertex2D ), (void *)offsetof( SVertex2D, vec2TexCoords ) );
}

void CGUITextData::DeleteText( void )
{
	if (m_glVAO != 0)
	{
		glDeleteVertexArrays( 1, &m_glVAO );
		m_glVAO = 0;
	}
	if (m_glVBO != 0)
	{
		glDeleteBuffers( 1, &m_glVBO );
		m_glVBO = 0;
	}
}

void CGUITextData::Draw( void )
{
	glBindVertexArray( m_glVAO );
	glDrawArrays( GL_TRIANGLES, 0, (GLsizei)m_verVertices.size() );
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

	m_vec2Bounds = g_vec2One;
	m_flLineScale = 1.0f;
	m_eTextAlign = ETextAlign::t_left;
}

CGUIText::~CGUIText()
{
	if (m_sText)
		delete[] m_sText;

	DeleteText();
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
		RecalculateText();

	BaseClass::PreThink();
}

void CGUIText::Draw( void )
{
	BaseClass::Draw();

	m_hTextMaterial->Apply();
	m_tTextInfo.Draw();
}

bool CGUIText::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_hTextMaterial->ShouldApply();
}

bool CGUIText::Insert( char cChar, unsigned int uiIndex )
{
	if (uiIndex <= GetTextLength())
	{
		m_cText.insert( m_cText.begin() + uiIndex, cChar );
		RecalculateText();
		return true;
	}

	return false;
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
		RecalculateText();
		return true;
	}

	return false;
}

bool CGUIText::Delete( unsigned int uiCount, unsigned int uiIndex )
{
	if (uiIndex + uiCount - 1 < GetTextLength())
	{
		m_cText.erase( m_cText.begin() + uiIndex, m_cText.begin() + uiIndex + uiCount );
		RecalculateText();
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
	RecalculateText();
}

void CGUIText::RecalculateText( void )
{
	DeleteText();
	CalculateText();
}

// TODO: clean up and finish (BLOCK TEXT NOT DONE)
void CGUIText::CalculateText( void )
{
	CFont *pFont = m_hTextMaterial->GetFont();
	float flTextScale = 2.0f / (float)(HasFlags( fl_absolute.GetFlag() ) ? DEFAULT_SCR_WIDTH : cv_r_windowsize.GetValue().x);
	float flLineSize = (float)pFont->GetFontSize() * m_flLineScale * flTextScale;

	CGUITextGeneration tgTextGeneration( m_cText, m_vec2Bounds, m_eTextAlign, flTextScale, flLineSize, pFont );
	tgTextGeneration.Generate( m_tTextInfo );

	m_tTextInfo.CreateText();
}

void CGUIText::DeleteText( void )
{
	m_tTextInfo.DeleteText();
}

const glm::vec2 &CGUIText::GetAlignFactor( void ) const
{
	return m_vec2Bounds;
}