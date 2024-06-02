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

	int BindBitmap( void ) const;

	unsigned int GetFontSize( void ) const;
	float GetAdvance( char cChar, float flScale ) const;
	glm::vec2 GetOffset( char cChar, float flScale ) const;
	glm::vec2 GetSize( char cChar, float flScale ) const;
	glm::vec2 GetNormalizedPosition( char cChar ) const;
	glm::vec2 GetNormalizedSize( char cChar ) const;

	void DrawChar( char cChar ) const;

private:
	CFontResource *m_pFontResource;
};

#endif // FONT_H