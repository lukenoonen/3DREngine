#include "GUIText.h"
#include "RenderManager.h"
#include "BaseCamera.h"

// TODO: clean up, make GUI elements render above everything else, and so on

DEFINE_DATADESC( CGUIText )

	DEFINE_FIELD( DataField, char *, m_sText, "text", FL_REQUIRED )

	DEFINE_FIELD( LinkedDataField, CHandle<CTextMaterial>, m_hTextMaterial, "textmaterial", FL_REQUIRED )

	DEFINE_FIELD( DataField, glm::vec2, m_vec2Bounds, "bounds", 0 )
	DEFINE_FIELD( DataField, float, m_flLineScale, "linescale", 0 )
	DEFINE_FIELD( DataField, ETextAlign, m_eTextAlign, "textalign", 0 )

END_DATADESC()

DEFINE_LINKED_CLASS( CGUIText, text_gui )

CGUIText::CGUIText()
{
	m_vec2Bounds = g_vec2One;
	m_flLineScale = 1.0f;
	m_eTextAlign = ETextAlign::t_left;
}

CGUIText::~CGUIText()
{
	delete[] m_sText;
	DeleteText();
}

bool CGUIText::Init( void )
{
	if (!BaseClass::Init())
		return false;

	if (!m_hTextMaterial)
		return false;

	CalculateText();
	return true;
}

void CGUIText::PostThink( void )
{
	if (cv_r_windowsize.WasDispatched())
		RecalculateText();

	BaseClass::PostThink();
}

void CGUIText::Draw( void )
{
	CBaseCamera *pCamera = pEntityManager->GetCurrentCamera();
	SetRotation( glm::quat( glm::eulerAngles( pCamera->GetRotation() ) * glm::vec3( 1.0f, 1.0f, 1.0f ) ) * glm::quat( glm::radians( glm::vec3( 90.0f, 0.0f, 0.0f ) ) ) );

	pRenderManager->SetUniformBufferObject( EUniformBufferObjects::t_model, 0, &GetModelMatrix() ); // TODO: change "model" matrix to better name

	// TODO: Enabling and disabling depth testing was removed in liu of setting the Z coord in the vs to be -1.0. See if this is a valid solution
	pRenderManager->SetBlend( true );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	m_hTextMaterial->Apply();

	pRenderManager->SetUniform( "u_vecTranslation", GetPosition() );

	glBindVertexArray( m_glVAO );
	glDrawArrays( GL_TRIANGLES, 0, (GLsizei)m_verVertices.size() );

	glBlendFunc( GL_ONE, GL_ONE );

	pRenderManager->SetBlend( false );
}

bool CGUIText::ShouldDraw( void ) const
{
	return BaseClass::ShouldDraw() && m_hTextMaterial->ShouldApply();
}

void CGUIText::SetText( const char *sText )
{
	delete[] m_sText;
	m_sText = UTIL_strdup( sText );
	RecalculateText();
}

void CGUIText::RecalculateText( void )
{
	DeleteText();
	CalculateText();
}

// TODO: clean up and finish (NOT DONE AT ALL)
void CGUIText::CalculateText( void )
{
	glm::vec2 vec2Cursor = g_vec2Zero;

	const char *sCursor = m_sText;

	float flTextScale = 2.0f / (float)cv_r_windowsize.GetValue().x;
	glm::vec2 vec2AdjustedBounds = m_vec2Bounds / flTextScale;

	CFont *pFont = m_hTextMaterial->GetFont();

	float flLine = (float)pFont->GetFontSize() * m_flLineScale;

	unsigned int uiLines = 0;
	std::stack<SGUITextChar> tcText;

	float flTest = 0.0f;

	while (*sCursor)
	{
		const char *sTarget = sCursor;
		float flAdvance = 0.0f;
		switch (*sCursor)
		{
		case '\n':
		{
			flAdvance = 0.0f;
			sTarget++;
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
			sTarget++;
			break;
		}
		case ' ':
		{
			flAdvance += (float)pFont->GetChar( *sTarget ).iAdvance;
			sTarget++;
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
					flAdvance += (float)pFont->GetChar( *sTarget ).iAdvance;
					sTarget++;
					switch (*sTarget)
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
				flAdvance += (float)pFont->GetChar( *sTarget ).iAdvance;
				sTarget++;
				break;
			}
			}

			break;
		}
		}

		if (vec2Cursor.x + flAdvance >= vec2AdjustedBounds.x || flAdvance == 0.0f)
		{
			vec2Cursor.y += flLine;

			if (vec2Cursor.y >= vec2AdjustedBounds.y)
				break;

			uiLines++;
			tcText.push( { '\0', flTest } );

			vec2Cursor.x = 0.0f;
		}

		switch (*sCursor)
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
		while (sCursor < sTarget)
		{
			switch (*sCursor)
			{
			case ' ':
			case '\t':
			case '\n':
			{
				break;
			}
			default:
			{
				const SChar &sChar = pFont->GetChar( *sCursor );
				tcText.push( { *sCursor, flCursorX } );

				flCursorX += (float)sChar.iAdvance;
				break;
			}
			}

			sCursor++;
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
			vec2Min *= g_vec2FlipVertical * flTextScale;
			vec2Max *= g_vec2FlipVertical * flTextScale;

			glm::vec2 vec2TexMin = (glm::vec2)sChar.vec2Position / (glm::vec2)m_hTextMaterial->GetFont()->GetBitmapSize();
			glm::vec2 vec2TexMax = vec2TexMin + (glm::vec2)sChar.vec2Size / (glm::vec2)m_hTextMaterial->GetFont()->GetBitmapSize();

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