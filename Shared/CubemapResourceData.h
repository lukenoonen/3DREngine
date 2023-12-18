#ifndef CUBEMAPRESOURCEDATA_H
#define CUBEMAPRESOURCEDATA_H

#include "Global.h"

struct SCubemapResourceData
{
	bool bFiltered;
	int iWidth[6];
	int iHeight[6];
	unsigned char ucChannels[6];
	unsigned char *pData[6];
};

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, SCubemapResourceData &dData );
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, SCubemapResourceData &dData );
#endif // PROJECT_RESOURCE

#endif // CUBEMAPRESOURCEDATA_H