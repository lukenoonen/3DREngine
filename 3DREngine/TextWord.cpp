#include "TextWord.h"

CTextWord::CTextWord( float flFontSize )
{
	m_flFontSize = flFontSize;
}

float CTextWord::GetWidth( void ) const
{
	return m_flWidth;
}

void CTextWord::AddTextChar( CTextChar *pTextChar )
{
	m_pTextChars.push_back( pTextChar );
	m_flWidth += (float)pTextChar->GetXAdvance() * m_flFontSize;
}

CTextChar *CTextWord::GetTextChar( unsigned int uiIndex ) const
{
	return m_pTextChars[uiIndex];
}

unsigned int CTextWord::GetTextCharCount( void ) const
{
	return (unsigned int)m_pTextChars.size();
}