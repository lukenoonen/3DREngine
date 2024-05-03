#include "GUIText.h"
#include "RenderManager.h"

// TODO: clean up, make GUI elements render above everything else, and so on

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
	m_vec2Bounds = g_vec2One;
	m_flLineScale = 1.0f;
	m_eTextAlign = ETextAlign::t_left;
	m_sText = NULL;
}

CGUIText::~CGUIText()
{
	DeleteText();
}

bool CGUIText::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (m_sText)
	{
		const char *pSearch = m_sText;
		while (*pSearch) m_cText.push_back( *pSearch++ );
		delete[] m_sText;
	}

	m_cText.push_back( '\0' );

	CalculateText();
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

	pRenderManager->SetUniform( "u_vecTranslation", GetPosition() );

	glBindVertexArray( m_glVAO );
	glDrawArrays( GL_TRIANGLES, 0, (GLsizei)m_verVertices.size() );
}

bool CGUIText::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_hTextMaterial->ShouldApply();
}

/* void CGUIText::SetText(const char *sText)
{
	delete[] m_sText;
	m_sText = UTIL_strdup( sText );
	RecalculateText();
}*/


unsigned int CGUIText::Insert( unsigned int uiPosition, char cChar )
{
	if (uiPosition <= GetTextLength())
	{
		m_cText.insert( m_cText.begin() + uiPosition, cChar );
		RecalculateText();
		return uiPosition + 1;
	}
	return uiPosition;
}

unsigned int CGUIText::Delete( unsigned int uiPosition )
{
	if (uiPosition < GetTextLength())
	{
		m_cText.erase( m_cText.begin() + uiPosition );
		RecalculateText();
	}

	return uiPosition;
}

unsigned int CGUIText::Backspace( unsigned int uiPosition )
{
	Delete( uiPosition - 1 );
	return uiPosition == 0 ? 0 : uiPosition - 1;
}

unsigned int CGUIText::GetLeft( unsigned int uiPosition ) const
{
	unsigned int uiLeft = uiPosition - 1;
	return uiLeft < GetTextLength() ? uiLeft : uiPosition;
}

unsigned int CGUIText::GetRight( unsigned int uiPosition ) const
{
	unsigned int uiRight = uiPosition + 1;
	return uiRight < GetTextLength() ? uiRight : uiPosition;
}

unsigned int CGUIText::GetUp( unsigned int uiPosition ) const
{
	// TODO: implement
	unsigned int uiLeft = uiPosition - 1;
	return uiLeft < GetTextLength() ? uiLeft : uiPosition;
}

unsigned int CGUIText::GetDown( unsigned int uiPosition ) const
{
	// TODO: implement
	unsigned int uiRight = uiPosition + 1;
	return uiRight < GetTextLength() ? uiRight : uiPosition;
}

unsigned int CGUIText::GetTextLength( void ) const
{
	return (unsigned int)m_cText.size() - 1;
}

void CGUIText::RecalculateText( void )
{
	DeleteText();
	CalculateText();
}

#include <iostream>

// TODO: clean up and finish (NOT DONE AT ALL)
void CGUIText::CalculateText( void )
{
	glm::vec2 vec2Cursor = g_vec2Zero;

	unsigned int uiCursor = 0;

	float flTextScale = 2.0f / (float)cv_r_windowsize.GetValue().x;
	glm::vec2 vec2AdjustedBounds = m_vec2Bounds / flTextScale;

	CFont *pFont = m_hTextMaterial->GetFont();

	float flLine = (float)pFont->GetFontSize() * m_flLineScale;

	unsigned int uiLines = 0;
	std::stack<SGUITextChar> tcText;

	float flTest = 0.0f;

	while (uiCursor < GetTextLength())
	{
		unsigned int uiTarget = uiCursor;
		float flAdvance = 0.0f;
		switch (m_cText[uiCursor])
		{
		case '\n':
		{
			flAdvance = 0.0f;
			uiTarget++;
			break;
		}
		case '\t':
		{
			// TODO: clean this
			float flTabAdvance = (float)m_hTextMaterial->GetFont()->GetChar( ' ' ).iAdvance * 4.0f;
			float flCursorDelta = (ceilf( vec2Cursor.x / flTabAdvance )) * flTabAdvance - vec2Cursor.x;
			if (flCursorDelta == 0.0f)
				flCursorDelta = flTabAdvance;
			flAdvance += flCursorDelta;
			uiTarget++;
			break;
		}
		case ' ':
		{
			flAdvance += (float)pFont->GetChar( m_cText[uiTarget] ).iAdvance;
			uiTarget++;
			break;
		}
		default:
		{
			switch (m_eTextAlign)
			{
			case ETextAlign::t_left:
			case ETextAlign::t_center:
			case ETextAlign::t_right:
			{
				bool bSearching = true;
				while (bSearching)
				{
					flAdvance += (float)pFont->GetChar( m_cText[uiTarget] ).iAdvance;
					uiTarget++;
					switch (m_cText[uiTarget])
					{
					case ' ':
					case '\t':
					case '\n':
					case '\0':
					{
						bSearching = false;
						break;
					}
					}
				}
				break;
			}
			default:
			{
				flAdvance += (float)pFont->GetChar( m_cText[uiTarget] ).iAdvance;
				uiTarget++;
				break;
			}
			}

			break;
		}
		}

		if (flAdvance == 0.0f || vec2Cursor.x + flAdvance >= vec2AdjustedBounds.x && !tcText.empty() && tcText.top().cChar != '\0')
		{
			vec2Cursor.y += flLine;

			if (m_eTextAlign != ETextAlign::t_continuous && vec2Cursor.y >= vec2AdjustedBounds.y)
				break;

			uiLines++;
			tcText.push( { '\0', flTest } );

			vec2Cursor.x = 0.0f;
		}

		switch (m_cText[uiCursor])
		{
		case ' ':
		case '\t':
		case '\n':
		{
			break;
		}
		default:
		{
			flTest = vec2Cursor.x + flAdvance;
			break;
		}
		}

		float flCursorX = vec2Cursor.x;
		while (uiCursor < uiTarget)
		{
			switch (m_cText[uiCursor])
			{
			case ' ':
			case '\t':
			case '\n':
			{
				break;
			}
			default:
			{
				const SChar &sChar = pFont->GetChar( m_cText[uiCursor] );
				tcText.push( { m_cText[uiCursor], flCursorX } );

				flCursorX += (float)sChar.iAdvance;
				break;
			}
			}

			if (flCursorX > vec2AdjustedBounds.x)
			{
				uiCursor = uiTarget;
				break;
			}

			uiCursor++;
		}

		vec2Cursor.x += flAdvance;
	}

	uiLines++;
	tcText.push( { '\0', flTest } );

	float flJustification = 0.0f;
	while (!tcText.empty())
	{
		if (tcText.top().cChar == '\0')
		{
			uiLines--;
			switch (m_eTextAlign)
			{
			case ETextAlign::t_right:
			{
				flJustification = vec2AdjustedBounds.x - tcText.top().flXPos;
				break;
			}
			case ETextAlign::t_center:
			{
				flJustification = (vec2AdjustedBounds.x - tcText.top().flXPos) * 0.5f;
				break;
			}
			default:
			{
				flJustification = 0.0f;
				break;
			}
			}
		}
		else
		{
			const SChar &sChar = m_hTextMaterial->GetFont()->GetChar( tcText.top().cChar );
			glm::vec2 vec2Min = glm::vec2( tcText.top().flXPos + flJustification, (float)(uiLines + 1) * flLine) + (glm::vec2)sChar.vec2Offset;
			glm::vec2 vec2Max = vec2Min + (glm::vec2)sChar.vec2Size;
			glm::vec2 vec2BoundsAdjust = g_vec2One;
			if (vec2Max.x > vec2AdjustedBounds.x)
			{
				vec2BoundsAdjust.x = (vec2AdjustedBounds.x - vec2Min.x) / (vec2Max.x - vec2Min.x);
				vec2Max.x = vec2AdjustedBounds.x;
			}
			if (vec2Max.y > vec2AdjustedBounds.y)
			{
				vec2BoundsAdjust.y = (vec2AdjustedBounds.y - vec2Min.y) / (vec2Max.y - vec2Min.y);
				vec2Max.y = vec2AdjustedBounds.y;
			}

			vec2Min *= g_vec2FlipVertical * flTextScale;
			vec2Max *= g_vec2FlipVertical * flTextScale;

			glm::vec2 vec2TexMin = (glm::vec2)sChar.vec2Position / (glm::vec2)m_hTextMaterial->GetFont()->GetBitmapSize();
			glm::vec2 vec2TexMax = vec2TexMin + (glm::vec2)sChar.vec2Size / (glm::vec2)m_hTextMaterial->GetFont()->GetBitmapSize() * vec2BoundsAdjust;

			m_verVertices.push_back( { glm::vec2( vec2Min.x, vec2Min.y ), glm::vec2( vec2TexMin.x, vec2TexMin.y ) } );
			m_verVertices.push_back( { glm::vec2( vec2Min.x, vec2Max.y ), glm::vec2( vec2TexMin.x, vec2TexMax.y ) } );
			m_verVertices.push_back( { glm::vec2( vec2Max.x, vec2Max.y ), glm::vec2( vec2TexMax.x, vec2TexMax.y ) } );
			m_verVertices.push_back( { glm::vec2( vec2Max.x, vec2Max.y ), glm::vec2( vec2TexMax.x, vec2TexMax.y ) } );
			m_verVertices.push_back( { glm::vec2( vec2Max.x, vec2Min.y ), glm::vec2( vec2TexMax.x, vec2TexMin.y ) } );
			m_verVertices.push_back( { glm::vec2( vec2Min.x, vec2Min.y ), glm::vec2( vec2TexMin.x, vec2TexMin.y ) } );
		}

		tcText.pop();
	}

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

void CGUIText::DeleteText( void )
{
	// TODO: See if checks for == 0 should be put back
	m_verVertices.clear();
	glDeleteVertexArrays( 1, &m_glVAO );
	glDeleteBuffers( 1, &m_glVBO );
}

const glm::vec2 &CGUIText::GetAlignFactor( void ) const
{
	return m_vec2Bounds;
}