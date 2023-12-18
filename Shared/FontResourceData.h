#ifndef FONTRESOURCEDATA_H
#define FONTRESOURCEDATA_H

#include "Global.h"

#define FONT_CHAR_START		0
#define FONT_CHAR_END		128
#define FONT_CHAR_SIZE		(FONT_CHAR_END - FONT_CHAR_START)

struct SChar
{
	glm::ivec2 vec2Position;
	glm::ivec2 vec2Size;
	glm::ivec2 vec2Offset;
	int iAdvance;
};

struct SFontResourceData
{
	unsigned int uiFontSize;
	glm::ivec2 vec2BitmapSize;
	unsigned char *pBitmapData;
	SChar sChars[FONT_CHAR_SIZE];
};

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, SFontResourceData &dData );
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, SFontResourceData &dData );
#endif // PROJECT_RESOURCE

#endif // FONTRESOURCEDATA_H