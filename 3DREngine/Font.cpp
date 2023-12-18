#include "Font.h"

DEFINE_DATADESC( CFont )

	DEFINE_FIELD( DataField, CFontResource *, m_pFontResource, "fontresource", FL_REQUIRED )

END_DATADESC()

DEFINE_LINKED_CLASS( CFont, asset_font )

CFont::CFont()
{

}

unsigned int CFont::GetFontSize( void ) const
{
	return m_pFontResource->GetFontSize();
}

const glm::ivec2 &CFont::GetBitmapSize( void ) const
{
	return m_pFontResource->GetBitmapSize();
}

int CFont::BindBitmap( void ) const
{
	return m_pFontResource->BindBitmap();
}

const SChar &CFont::GetChar( char cChar ) const
{
	return m_pFontResource->GetChar( cChar );
}