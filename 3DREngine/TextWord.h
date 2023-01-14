#ifndef TEXTWORD_H
#define TEXTWORD_H

#include "Global.h"
#include "TextChar.h"

class CTextWord
{
	DECLARE_CLASS_NOBASE( CTextWord )

	CTextWord( float flFontSize );

	float GetWidth( void ) const;

	void AddTextChar( CTextChar *pTextChar );
	CTextChar *GetTextChar( unsigned int uiIndex ) const;
	unsigned int GetTextCharCount( void ) const;

private:
	float m_flFontSize;

	float m_flWidth;

	std::vector<CTextChar *> m_pTextChars;
};

#endif // TEXTWORD_H