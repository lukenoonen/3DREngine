#include "Font.h"

DEFINE_DATADESC( CFont )

	DEFINE_FIELD( DataField, CFontResource *, m_pFontResource, "fontresource", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CFont, asset_font )

CFont::CFont()
{

}

int CFont::BindBitmap( void ) const
{
	return m_pFontResource->BindBitmap();
}

unsigned int CFont::GetFontSize( void ) const
{
	return m_pFontResource->GetFontSize();
}

float CFont::GetAdvance( char cChar, float flScale ) const
{
	return (float)m_pFontResource->GetChar( cChar ).iAdvance * flScale;
}

glm::vec2 CFont::GetOffset( char cChar, float flScale ) const
{
	return (glm::vec2)m_pFontResource->GetChar( cChar ).vec2Offset * flScale;
}

glm::vec2 CFont::GetSize( char cChar, float flScale ) const
{
	return (glm::vec2)m_pFontResource->GetChar( cChar ).vec2Size * flScale;
}

glm::vec2 CFont::GetNormalizedPosition( char cChar ) const
{
	return (glm::vec2)m_pFontResource->GetChar( cChar ).vec2Position / (glm::vec2)m_pFontResource->GetBitmapSize();
}

glm::vec2 CFont::GetNormalizedSize( char cChar ) const
{
	return (glm::vec2)m_pFontResource->GetChar( cChar ).vec2Size / (glm::vec2)m_pFontResource->GetBitmapSize();
}