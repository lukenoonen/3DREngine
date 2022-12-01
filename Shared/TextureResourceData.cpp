#include "TextureResourceData.h"

bool UTIL_GetValue( const CTextItem *pTextItem, ETextureWrap &eValue )
{
	const char *sName;
	if (!pTextItem->GetValue( sName ))
		return false;

	if (UTIL_streq( sName, "repeat" ))
		eValue = ETextureWrap::t_repeat;
	else if (UTIL_streq( sName, "mirrored" ))
		eValue = ETextureWrap::t_mirrored;
	else if (UTIL_streq( sName, "edge" ))
		eValue = ETextureWrap::t_edge;
	else if (UTIL_streq( sName, "border" ))
		eValue = ETextureWrap::t_border;
	else
		return false;

	return true;
}

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, STextureResourceData &dData )
{
	if (!pFile->Read( dData.bFiltered ))
		return false;

	if (!pFile->Read( dData.eTextureWrap ))
		return false;

	if (dData.eTextureWrap == ETextureWrap::t_border)
	{
		if (!pFile->Read( dData.vec4BorderColor ))
			return false;
	}

	if (!pFile->Read( dData.iWidth ))
		return false;

	if (!pFile->Read( dData.iHeight ))
		return false;

	if (!pFile->Read( dData.ucChannels ))
		return false;

	unsigned int uiSize = dData.iWidth * dData.iHeight * dData.ucChannels;
	dData.pData = new unsigned char[uiSize];
	if (!pFile->ReadBytes( dData.pData, uiSize ))
	{
		delete[] dData.pData;
		return false;
	}

	return true;
}
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE

bool UTIL_Write( CFile *pFile, STextureResourceData &dData )
{
	if (!pFile->Write( dData.bFiltered ))
		return false;

	if (!pFile->Write( dData.eTextureWrap ))
		return false;

	if (dData.eTextureWrap == ETextureWrap::t_border)
	{
		if (!pFile->Write( dData.vec4BorderColor ))
			return false;
	}

	if (!pFile->Write( dData.iWidth ))
		return false;

	if (!pFile->Write( dData.iHeight ))
		return false;

	if (!pFile->Write( dData.ucChannels ))
		return false;

	unsigned int uiSize = dData.iWidth * dData.iHeight * dData.ucChannels;
	if (!pFile->WriteBytes( dData.pData, uiSize ))
		return false;

	return true;
}

bool UTIL_GetValue( const CTextItem *pTextItem, STextureResourceData &dValue )
{

}
#endif // PROJECT_RESOURCE