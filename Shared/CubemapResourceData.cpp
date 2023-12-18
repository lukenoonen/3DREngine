#include "CubemapResourceData.h"

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, SCubemapResourceData &dData )
{
	if (!pFile->Read( dData.bFiltered ))
		return false;

	for (unsigned int i = 0; i < 6; i++)
	{
		if (!pFile->Read( dData.iWidth[i] ))
			return false;

		if (!pFile->Read( dData.iHeight[i] ))
			return false;

		if (!pFile->Read( dData.ucChannels[i] ))
			return false;

		unsigned int uiSize = dData.iWidth[i] * dData.iHeight[i] * dData.ucChannels[i];
		dData.pData[i] = new unsigned char[uiSize];
		if (!pFile->ReadBytes( dData.pData[i], uiSize ))
		{
			for (unsigned int j = 0; j <= i; j++)
				delete[] dData.pData[j];

			return false;
		}
	}

	return true;
}
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, SCubemapResourceData &dData )
{
	if (!pFile->Write( dData.bFiltered ))
		return false;

	for (unsigned int i = 0; i < 6; i++)
	{
		if (!pFile->Write( dData.iWidth[i]))
			return false;

		if (!pFile->Write( dData.iHeight[i] ))
			return false;

		if (!pFile->Write( dData.ucChannels[i] ))
			return false;

		unsigned int uiSize = dData.iWidth[i] * dData.iHeight[i] * dData.ucChannels[i];
		if (!pFile->WriteBytes( dData.pData[i], uiSize ))
			return false;
	}

	return true;
}
#endif // PROJECT_RESOURCE