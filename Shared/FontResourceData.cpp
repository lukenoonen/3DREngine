#include "FontResourceData.h"

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, SFontResourceData &dData )
{
	if (!pFile->Read( dData.uiFontSize ))
		return false;

	if (!pFile->Read( dData.vec2BitmapSize ))
		return false;

	unsigned int uiBitmapSize = dData.vec2BitmapSize.x * dData.vec2BitmapSize.y;
	dData.pBitmapData = new unsigned char[uiBitmapSize];
	if (!pFile->ReadBytes( dData.pBitmapData, uiBitmapSize ))
	{
		delete[] dData.pBitmapData;
		return false;
	}

	if (!pFile->ReadBytes( dData.sChars, FONT_CHAR_SIZE * sizeof( SChar ) )) // TODO: verify this works
	{
		delete[] dData.pBitmapData;
		return false;
	}

	return true;
}
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE

bool UTIL_Write( CFile *pFile, SFontResourceData &dData )
{
	if (!pFile->Write( dData.uiFontSize ))
		return false;

	if (!pFile->Write( dData.vec2BitmapSize ))
		return false;

	unsigned int uiBitmapSize = dData.vec2BitmapSize.x * dData.vec2BitmapSize.y;
	if (!pFile->WriteBytes( dData.pBitmapData, uiBitmapSize ))
		return false;

	if (!pFile->WriteBytes( dData.sChars, FONT_CHAR_SIZE * sizeof( SChar ) )) // TODO: verify this works
	{
		delete[] dData.pBitmapData;
		return false;
	}

	return true;
}
#endif // PROJECT_RESOURCE