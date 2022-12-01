#include "SkeletonResourceData.h"

#ifdef PROJECT_ENGINE
bool UTIL_ReadInternal( CFile *pFile, SSkeletonNode *&pData )
{
	pData = new SSkeletonNode();

	if (!pFile->Read( pData->uiIndex ))
		return false;

	bool bHasBone;

	if (!pFile->Read( bHasBone ))
		return false;

	if (bHasBone)
	{
		pData->pSkeletonBone = new SSkeletonBone();
		SSkeletonBone *pSkeletonBone = pData->pSkeletonBone;

		if (!pFile->Read( pSkeletonBone->uiIndex ))
			return false;

		if (!pFile->Read( pSkeletonBone->matOffset ))
			return false;
	}

	unsigned int uiChildrenCount;
	if (!pFile->Read( uiChildrenCount ))
		return false;

	pData->pChildren.resize( uiChildrenCount, NULL );

	for (unsigned int i = 0; i < uiChildrenCount; i++)
	{
		if (!UTIL_ReadInternal( pFile, pData->pChildren[i] ))
			return false;
	}

	return true;
}

bool UTIL_Read( CFile *pFile, SSkeletonResourceData &dData )
{
	if (!pFile->Read( dData.uiSkeletonBonesCount ))
		return false;

	if (dData.uiSkeletonBonesCount == 0)
	{
		dData.pSkeletonNode = NULL;
		return true;
	}

	bool bResult = UTIL_ReadInternal( pFile, dData.pSkeletonNode );

	if (!bResult)
		delete dData.pSkeletonNode;

	return bResult;
}
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_WriteInternal( CFile *pFile, SSkeletonNode *&pData )
{
	if (!pFile->Write( pData->uiIndex ))
		return false;

	SSkeletonBone *pSkeletonBone = pData->pSkeletonBone;
	bool bHasBone = pSkeletonBone != NULL;

	if (!pFile->Write( bHasBone ))
		return false;

	if (bHasBone)
	{
		if (!pFile->Write( pSkeletonBone->uiIndex ))
			return false;

		if (!pFile->Write( pSkeletonBone->matOffset ))
			return false;
	}

	unsigned int uiChildrenCount = (unsigned int)pData->pChildren.size();
	if (!pFile->Write( uiChildrenCount ))
		return false;

	for (unsigned int i = 0; i < uiChildrenCount; i++)
	{
		if (!UTIL_WriteInternal( pFile, pData->pChildren[i] ))
			return false;
	}

	return true;
}

bool UTIL_Write( CFile *pFile, SSkeletonResourceData &dData )
{
	if (!pFile->Write( dData.uiSkeletonBonesCount ))
		return false;

	if (dData.uiSkeletonBonesCount == 0)
		return true;

	return UTIL_WriteInternal( pFile, dData.pSkeletonNode );
}

bool UTIL_GetValue( const CTextItem *pTextItem, SSkeletonResourceData &dValue )
{

}
#endif // PROJECT_RESOURCE