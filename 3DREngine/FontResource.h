#ifndef FONTRESOURCE_H
#define FONTRESOURCE_H

#include "Global.h"
#include "BaseResource.h"
#include "FontResourceData.h"
#include "Vertex2D.h"

class CFontResource;

class CFontChar
{
public:
	DECLARE_CLASS_NOBASE( CFontChar )

	CFontChar( const SChar &sChar, CFontResource *pFontResource );
	~CFontChar();

	void Draw( void ) const;

	GLuint m_glVAO;
	GLuint m_glVBO;

private:
	SVertex2D m_verVertices[6];
};

class CFontResource : public CBaseResource
{
public:
	DECLARE_CLASS( CFontResource, CBaseResource )

	CFontResource( SFontResourceData &sData, const char *sName );
	virtual ~CFontResource();
	
	unsigned int GetFontSize( void ) const;
	const glm::ivec2 &GetBitmapSize( void ) const;
	int BindBitmap( void ) const;
	const SChar &GetChar( char cChar ) const;

	void DrawChar( char cChar );

private:
	unsigned int m_uiFontSize;
	glm::ivec2 m_vec2BitmapSize;
	GLuint m_glBitmapID;
	SChar m_sChars[FONT_CHAR_SIZE];
	CFontChar *m_pFontChars[FONT_CHAR_SIZE];
};

bool UTIL_Write( CFile *pFile, CFontResource *&pData );
bool UTIL_Read( CFile *pFile, CFontResource *&pData );
bool UTIL_GetValue( const CTextItem *pTextItem, CFontResource *&pValue );

#endif // FONTRESOURCE_H