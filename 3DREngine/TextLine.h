#ifndef TEXTLINE_H
#define TEXTLINE_H

#include "Global.h"
#include "TextWord.h"

class CTextBlock;

class CTextLine
{
public:
	DECLARE_CLASS_NOBASE( CTextLine )

	CTextLine( CTextBlock *m_);

private:
	CTextBlock *m_pTextBlock;

	std::vector<CTextWord *> m_pWords;
	float m_flWidth;
};

#endif // TEXTLINE_H