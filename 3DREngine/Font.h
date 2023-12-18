#ifndef FONT_H
#define FONT_H

#include "Global.h"
#include "BaseAsset.h"
#include "FontResource.h"

class CFont : public CBaseAsset
{
public:
	DECLARE_CLASS( CFont, CBaseAsset )

	DECLARE_DATADESC()

	DECLARE_LINKED_CLASS()

	CFont();

	unsigned int GetFontSize( void ) const;
	const glm::ivec2 &GetBitmapSize( void ) const;
	int BindBitmap( void ) const;
	const SChar &GetChar( char cChar ) const;

private:
	CFontResource *m_pFontResource;
};

#endif // FONT_H