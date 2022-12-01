#ifndef SKELETONRESOURCEDATA_H
#define SKELETONRESOURCEDATA_H

#include "Global.h"

struct SSkeletonBone
{
	unsigned int uiIndex;
	glm::mat4 matOffset;
};

struct SSkeletonNode
{
	SSkeletonNode()
	{
		pSkeletonBone = NULL;
	}

	~SSkeletonNode()
	{
		if (pSkeletonBone)
			delete pSkeletonBone;

		for (unsigned int i = 0; i < pChildren.size(); i++)
		{
			if (!pChildren[i])
				break;

			delete pChildren[i];
		}
	}

	unsigned int uiIndex;
	SSkeletonBone *pSkeletonBone;
	std::vector<SSkeletonNode *> pChildren;
};

struct SSkeletonResourceData
{
	unsigned int uiSkeletonBonesCount;
	SSkeletonNode *pSkeletonNode;
};

#ifdef PROJECT_ENGINE
bool UTIL_Read( CFile *pFile, SSkeletonResourceData &dData );
#endif // PROJECT_ENGINE

#ifdef PROJECT_RESOURCE
bool UTIL_Write( CFile *pFile, SSkeletonResourceData &dData );
bool UTIL_GetValue( const CTextItem *pTextItem, SSkeletonResourceData &dValue );
#endif // PROJECT_RESOURCE

#endif // SKELETONRESOURCEDATA_H